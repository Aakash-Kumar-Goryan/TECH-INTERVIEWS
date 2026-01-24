# Design System to Sync Hotel Availability (Channel Manager)

> **Model Interview Answer** - This document focuses on the consistency challenges (race conditions) and synchronization strategies required for a Channel Manager connecting hotels to OTAs (Online Travel Agencies like Booking.com, Airbnb).

---

## 1. Requirements Clarification (5 min)

### 1.1 Functional Requirements

1. **Inventory Management**: Hotels can define room availability (count) per Room Type per Date.
2. **Multi-Platform Sync**: When a booking occurs on one platform (e.g., Booking.com), inventory must be decremented across ALL other platforms (Airbnb, Expedia, Agoda, Direct Website).
3. **Booking Ingestion**: Receive bookings from OTAs and updating the central ledger.
4. **Rate Management**: (Secondary) Sync prices across platforms.
5. **Prevention**: Prevent double bookings (selling the same room to two different people).

### 1.2 Non-Functional Requirements

| Requirement | Target | Rationale |
|-------------|--------|-----------|
| **Consistency** | Strong/Eventual | Central DB must be strong; OTA sync is eventual but fast (< 2s). |
| **Availability** | 99.99% | Downtime = Lost revenue or overbookings. |
| **Throughput** | High Write | Inventory updates trigger fan-out API calls to multiple OTAs. |
| **Latency** | Low | Updates to OTAs must be near real-time to minimize risk window. |

### 1.3 Scope

* **In Scope**: Central inventory DB, Webhooks from OTAs, Pushing updates to OTAs, Locking mechanisms.
* **Out of Scope**: Payment processing, user reviews, hotel onboarding flows.

---

## 2. High-Level Architecture (10 min)

### 2.1 Component Diagram

```
                                      ┌──────────────────┐
                                      │   Hotel Staff    │
                                      └────────┬─────────┘
                                               │
                                      ┌────────▼─────────┐
                                      │   PMS (UI/API)   │◄──── Direct Bookings
                                      └────────┬─────────┘
                                               │
┌──────────────┐                      ┌────────▼─────────┐
│ OTA 1 (Bkg)  │◄──PUSH updates───────┤ Channel Manager  │
│              │──WEBHOOK (Booking)──►│     Core         │
└──────────────┘                      └────────┬─────────┘
                                               │
┌──────────────┐                      ┌────────▼─────────┐
│ OTA 2 (Abnb) │◄──PUSH updates───────┤  Availability    │
│              │──WEBHOOK (Booking)──►│   Database       │
└──────────────┘                      └──────────────────┘
```

### 2.2 Core Components

1. **Booking Service (Ingestion)**:
    * Receives webhooks from OTAs (e.g., "New Booking for Room X on Date Y").
    * Handles direct bookings from the hotel's own website.
2. **Inventory Service**:
    * Manages the "True Count" of rooms.
    * Enforces transactional integrity.
3. **Sync Engine (Fan-out)**:
    * When inventory changes, calculates the "Delta" for connected OTAs.
    * Pushes updates via OTA-specific Adapters.
4. **OTA Adapters**:
    * Encapsulate the specific API logic (XML/JSON, SOAP/REST) for each platform.
    * Handle auth, rate limits, and retries.

---

## 3. Data Model & Database Design

The heart of the system is the Inventory Table.

### 3.1 Inventory Table (Relational DB)

We need ACID properties to prevent local race conditions.

```sql
CREATE TABLE inventory (
    hotel_id        UUID,
    room_type_id    UUID,
    date            DATE,
    total_rooms     INT,    -- Physical capacity
    booked_count    INT,    -- Rooms sold
    reserved_count  INT,    -- Rooms explicitly held (pending)
    version         BIGINT, -- For Optimistic Locking
    
    PRIMARY KEY (hotel_id, room_type_id, date),
    -- Constraint to prevent overbooking at DB level
    CHECK (booked_count + reserved_count <= total_rooms)
);
```

### 3.2 Bookings Table

```sql
CREATE TABLE bookings (
    booking_id      VARCHAR PRIMARY KEY, -- OTA specific ID
    ota_source      VARCHAR,             -- 'booking.com', 'airbnb'
    status          VARCHAR,             -- 'CONFIRMED', 'CANCELLED'
    created_at      TIMESTAMP
);
```

---

## 4. Handling Race Conditions (The Interview Core)

The critical challenge: **The "Last Room" Problem.**

* Inventory: 1 room left.
* User A on Booking.com clicks "Book".
* User B on Airbnb clicks "Book" at the exact same second.

### 4.1 Strategy 1: Database Optimistic Locking (Internal)

When receiving a booking request (either direct or via OTA webhook):

```sql
-- Attempt to increment booked_count
UPDATE inventory
SET booked_count = booked_count + 1,
    version = version + 1
WHERE hotel_id = ? 
  AND room_type_id = ? 
  AND date = ? 
  AND (booked_count + reserved_count) < total_rooms; -- The Guard Rail
```

* **Result**: If `RowsAffected == 0`, the room is gone.
  * **Action**: Reject the booking.

### 4.2 Handling OTA Latency (External Race Conditions)

We cannot lock the OTA's database. By the time Booking.com sends us the webhook, they have already told User A "Success".

**Scenario**:

1. Inventory = 1.
2. Booking.com sells the room. Sends Webhook.
3. We update DB: Inventory = 0.
4. Async Process starts to sync Airbnb.
5. *Before* Airbnb receives `Inventory=0`, Airbnb sells the room. Airbnb sends Webhook.
6. We try to update DB. **FAIL** (Capacity exceeded).

**Solutions**:

#### A. Soft Blocks / Reservations (Preventative)

If API allows, when a user enters the "checkout flow", try `Reserve` on our system.

* *Pros*: Strong consistency.
* *Cons*: Most OTAs do not support a "Prepare Phase" call to the hotel's system during checkout due to latency sensitivity.

#### B. Overbooking Buffer (Risk Management)

Always hide 1 room from OTAs if volatility is high.

* *Real Inventory*: 1.
* *Broadcast to OTAs*: 0.
* *Pros*: Safe.
* *Cons*: Revenue loss (unsold last room).

#### C. Detective & Compensating Transactions (Industry Standard)

Accept that overbookings **will** happen.

1. **Detect**: When Airbnb sends the webhook, our DB update fails (Inventory -1).
2. **Action**:
    * Log "Overbooking Event".
    * Immediate manual/auto alert to Hotel Manager.
    * **Auto-Resolution**:
        * Check other Room Types (Upgrade?).
        * Call OTA API to trigger "Relocation" flow (OTA finds nearby hotel).
3. **Optimization**:
    * Prioritize the "Update" queue.
    * Websockets/Push notifications to OTAs > Polling.

---

## 5. Synchronization Workflow (Sync Engine)

Speed is the antidote to race conditions.

### 5.1 The Flow

1. **Trigger**: `Inventory` table changes (Update/Insert trigger or App Event).
2. **Event**: Publish `InventoryChangedEvent(hotel_id, room_id, date, new_count)` to Kafka.
3. **Consumer**: `SyncWorker` consumes event.
4. **Transformation**:
    * Fetch all connected OTAs for this hotel.
    * Apply mapping (Our `RoomID: Double` -> Booking.com `RoomID: 12345`).
5. **Fan-Out**:
    * Push updates to OTA APIs in parallel.

### 5.2 Queue Isolation

Use separate queues for different OTAs?

* **Yes**. Booking.com might be down or slow. We don't want that to block updates to Airbnb.
* *Structure*: `Kafka Topic: channel-updates` -> Consumer Group per Adapter.

### 5.3 Rate Limiting

OTAs impose strict rate limits (e.g., 5 requests/sec).

* **Problem**: A bulk update (Open 365 days) = 365 API calls.
* **Solution**: **Batching**.
  * Most OTAs support "Bulk Update" endpoints.
  * Aggregator: Wait 500ms, collect all date updates, send 1 JSON payload.

---

## 6. Detailed Design: Idempotency & Order

Updates must arrive in order.

* **Problem**:
  * t=1: Update Inventory to 5.
  * t=2: Update Inventory to 4.
  * Network creates race. OTA receives "4" then "5".
  * Result: OTA shows 5 (Wrong).
* **Solution**: Timestamp / Versioning.
  * Send `sequence_id` or `generated_at` timestamp in the API payload.
  * Design Adapters to ignore older timestamps if the OTA supports it.
  * If OTA doesn't support versioning: Enforce strict ordering in the specific OTA queue (Partition by HotelID).

---

## 7. Scaling Constraints

### 7.1 Database Capacity

* 5M Rooms * 365 Days = ~1.8 Billion rows.
* **Partitioning**: Partition `Inventory` table by `Hotel_ID`.
* **Sharding**: Shard the DB by `Hotel_ID` (Hotels don't interact with each other).

### 7.2 Thundering Herd

* Hotel Manager updates "Season Pricing" for next year.
* 1 Hotel *10 Room Types* 365 Days = 3,650 updates generated instantly.
* 10 OTAs connected = 36,500 API calls.
* **Solution**:
  * **Debouncing**: Don't sync individual row updates. Sync the "Transaction".
  * **Diffing**: Only send what changed.

---

## 8. Summary Checklist

* **DB**: Relational (Postgres) with Sharding by Hotel.
* **Locking**: Optimistic Locking (Version column) for internal consistency.
* **Race Conditions**:
  * *Internal*: Handled by DB constraints.
  * *External (OTA)*: Minimized by fast sync (Kafka), handled by Compensating Transactions (Upgrades/Relocation).
* **Sync**:
  * Event-Driven Architecture.
  * Batched API calls.
  * Queue isolation per OTA to prevent Head-of-Line blocking.
