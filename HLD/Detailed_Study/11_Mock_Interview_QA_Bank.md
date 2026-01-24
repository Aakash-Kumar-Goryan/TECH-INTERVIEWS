# Mock Interview Q&A Bank

> **Purpose**: Challenging follow-up questions interviewers love to ask, with model answers.

---

## How to Use This Guide

1. **After studying a design**: Test yourself with these questions
2. **During mock interviews**: Use as interviewer prompts
3. **Focus on trade-offs**: Most questions probe decision rationale

---

## 1. URL Shortener

### Q1: What happens if your key generation service fails?

> **Model Answer**:
>
> - Pre-generate keys in batches (100K at a time) and store in a "key pool" table
> - Multiple app servers pull from this pool with atomic transactions
> - If ZooKeeper/counter service fails, we can still serve from pool for hours
> - Fallback: Use UUID + Base62 (longer URLs but works)

### Q2: How do you handle a viral short URL getting 1M hits/minute?

> **Model Answer**:
>
> - CDN caching at edge (Cloudflare, Akamai) - 99% cache hit for popular URLs
> - Read replicas of the mapping database
> - Consistent hashing ensures same URL hits same cache shard
> - Rate limiting per IP to prevent abuse

### Q3: How do you prevent enumeration attacks?

> **Model Answer**:
>
> - Don't use sequential IDs - use random Base62 keys
> - Minimum key length of 6 chars = 56B combinations
> - Rate limit per IP on redirect endpoint
> - CAPTCHA after N failed lookups from same source

### Q4: What if someone creates malicious redirect URLs?

> **Model Answer**:
>
> - URL validation at creation time (check against blacklists)
> - Integration with Google Safe Browsing API
> - Delay redirect with interstitial warning for flagged domains
> - Report mechanism for users, automated takedown

### Q5: How do you handle URL expiration efficiently?

> **Model Answer**:
>
> - TTL stored with each mapping, check on read
> - Background job with TTL index for batch deletion
> - Don't delete immediately - soft delete, then hard delete after grace period
> - Reuse expired keys after 30-day quarantine

---

## 2. Uber / Ride-Sharing

### Q1: How do you prevent location spoofing by drivers?

> **Model Answer**:
>
> - Cross-check GPS with cell tower triangulation
> - Validate speed between location updates (can't teleport)
> - Machine learning on movement patterns
> - Periodic photo verification during ride

### Q2: What if the matching service goes down?

> **Model Answer**:
>
> - Multiple matching service instances behind load balancer
> - If all down: graceful degradation to "no drivers available"
> - Drivers stay assigned to last known ride until timeout
> - Queue rider requests, process when service recovers

### Q3: How do you handle a driver accepting a ride but never arriving?

> **Model Answer**:
>
> - ETA monitoring - if no progress in X minutes, auto-cancel
> - Rider can cancel, driver gets soft penalty
> - After N incidents, driver flagged for review
> - Rider gets priority in next matching

### Q4: How do you estimate surge pricing in real-time?

> **Model Answer**:
>
> - Demand = ride requests per S2 cell per minute
> - Supply = available drivers in same cell
> - Surge multiplier = Demand / Supply (with caps)
> - Smoothing: average over 5-minute window to prevent spikes
> - Show surge to rider BEFORE they confirm

### Q5: How do you handle payment failures mid-ride?

> **Model Answer**:
>
> - Pre-authorize card at ride start (hold $50-100)
> - Final charge at ride end with actual amount
> - If final charge fails: retry with exponential backoff
> - After 3 failures: flag account, allow completion of ride
> - Collections process for unpaid rides

---

## 3. WhatsApp / Messaging

### Q1: How do you ensure E2E encryption keys aren't compromised?

> **Model Answer**:
>
> - Keys generated on device, never sent to server
> - Signal Protocol: new session key per message (forward secrecy)
> - Key verification via QR code / fingerprint comparison
> - If device compromised, only that device's messages exposed

### Q2: What if the message server is overloaded?

> **Model Answer**:
>
> - Message queues (Kafka) buffer incoming messages
> - Backpressure: slow down accepts when queue grows
> - Priority lanes: transactional messages skip marketing queue
> - Graceful degradation: increase delivery latency, never drop

### Q3: How do you sync messages across multiple devices?

> **Model Answer**:
>
> - Primary device is source of truth
> - Linked devices get copy of session keys encrypted to their device key
> - Messages fan-out to all linked devices
> - Challenge: E2E encryption with multiple endpoints requires multi-device Signal Protocol

### Q4: How do you handle spam/abuse in encrypted messages?

> **Model Answer**:
>
> - Can't read content, but can analyze metadata:
>   - Message frequency (rapid-fire = bot)
>   - Network graph (messaging strangers)
>   - Report signals from recipients
> - User-side ML model that runs locally (doesn't send content)
> - Account limits for new users (can't message 1000 people day 1)

### Q5: How do you guarantee message ordering?

> **Model Answer**:
>
> - Sequence number per sender-receiver pair
> - Server assigns timestamp, client reorders by sequence
> - If gap in sequence, request missing message
> - Vector clocks for group messages (partial ordering)

---

## 4. Instagram / Social Feed

### Q1: How do you prevent feed manipulation by bots?

> **Model Answer**:
>
> - Device fingerprinting + behavioral analysis
> - Rate limiting on likes/follows per account
> - ML model on engagement patterns (humans don't like 1000 posts/hour)
> - Shadow banning: reduce reach without explicit block

### Q2: What if the fan-out service is behind?

> **Model Answer**:
>
> - Monitor fan-out queue depth
> - For celebrities (>1M followers), switch to pull-based
> - Accept delayed feed updates for non-time-sensitive posts
> - Priority to active users (recently opened app)

### Q3: How do you handle a user with 100M followers posting?

> **Model Answer**:
>
> - NO fan-out on write for mega-influencers
> - Pull-based: when follower opens app, query influencer's recent posts
> - Cache influencer's post list (10 posts fits in Redis)
> - Merge with pre-computed feed on client

### Q4: How do you rank posts in the feed?

> **Model Answer**:
>
> - Signals: recency, engagement rate, relationship strength, content type preference
> - ML model trained on "time spent" as proxy for interest
> - Real-time features: what user engaged with in last 5 minutes
> - A/B test constantly, optimize for retention metrics

### Q5: How do you handle image moderation at scale?

> **Model Answer**:
>
> - ML classifier on upload (nudity, violence, copyright)
> - Human review queue for borderline cases
> - Hash matching against known bad content (PhotoDNA)
> - User reports surface to moderation queue
> - Appeals process with human review

---

## 5. YouTube / Netflix (Video Streaming)

### Q1: How do you prevent video piracy?

> **Model Answer**:
>
> - DRM: Widevine (Chrome/Android), FairPlay (Apple), PlayReady (Microsoft)
> - Video encrypted at rest, decrypted only on licensed player
> - Watermarking: invisible user ID in video frames
> - Token authentication: signed URLs expire quickly

### Q2: What if a data center goes down during Super Bowl?

> **Model Answer**:
>
> - Multi-region deployment with active-active
> - DNS failover to healthy region (Route53 health checks)
> - Edge caches (CDN) serve most content without origin
> - Pre-warm caches for known big events

### Q3: How do you handle a video going viral unexpectedly?

> **Model Answer**:
>
> - CDN auto-scales to demand
> - Popular content auto-promoted to edge caches
> - Origin shielding: only one CDN request per cache miss
> - If extreme: temporarily reduce quality options

### Q4: How do you minimize rebuffering on slow connections?

> **Model Answer**:
>
> - ABR (Adaptive Bitrate): client measures bandwidth, requests appropriate quality
> - Buffer 30-60 seconds ahead
> - Start with low quality, ramp up
> - Predict bandwidth drops (ML on network patterns)

### Q5: How do you transcode efficiently for 4K content?

> **Model Answer**:
>
> - Per-title encoding: analyze content, allocate bits where needed
> - Dark scenes need fewer bits than action
> - Parallel encoding: split video into chunks, encode concurrently
> - Hardware encoders for high-volume (NVENC, VPU)

---

## 6. Typeahead / Autocomplete

### Q1: How do you prevent suggestion manipulation?

> **Model Answer**:
>
> - Don't let single user inflate query frequency (1 count per user per query per day)
> - Rate limiting on query logging
> - Human review for top suggestions
> - Blacklist offensive terms

### Q2: What if the trie becomes corrupted?

> **Model Answer**:
>
> - Immutable trie snapshots (rebuild, don't mutate in place)
> - Blue-green deployment: new trie tested before swap
> - Keep last 3 snapshots for rollback
> - Validation: compare top-K for sample queries before deploy

### Q3: How do you handle trending topics in real-time?

> **Model Answer**:
>
> - Separate "trending trie" updated every minute
> - Count-Min Sketch for approximate frequency
> - Merge trending results with main trie at query time
> - Boost factor for trending (2x weight)

### Q4: How do you support multiple languages?

> **Model Answer**:
>
> - Separate trie per language
> - Detect language from user locale or query script
> - Mixed-language: query multiple tries, merge results
> - Transliteration: "tokyo" should match "東京"

### Q5: How do you reduce latency below 50ms?

> **Model Answer**:
>
> - Trie in memory (no disk I/O)
> - Precomputed top-K at each node (no DFS on query)
> - Client-side debouncing (100ms) reduces server load
> - Edge deployment: trie servers in every region

---

## 7. Web Crawler

### Q1: How do you handle JavaScript-rendered pages?

> **Model Answer**:
>
> - Headless browser farm (Puppeteer, Playwright)
> - Expensive: only for high-value pages
> - Heuristic: if initial HTML has little content, queue for JS rendering
> - Cache rendered HTML to avoid re-rendering

### Q2: What if a website blocks your crawler?

> **Model Answer**:
>
> - Respect robots.txt - if blocked, don't crawl
> - Don't try to circumvent (ethical + legal)
> - Lower crawl rate if getting 429s
> - Contact site owner if critical content

### Q3: How do you detect and avoid spider traps?

> **Model Answer**:
>
> - Max URL length (2048 chars)
> - Max depth per domain (e.g., 15 levels)
> - URL pattern detection (calendar traps: /2024/01/01, /2024/01/02...)
> - Bloom filter for URL dedup

### Q4: How do you prioritize what to crawl?

> **Model Answer**:
>
> - PageRank or similar authority score
> - Freshness: news sites more often than static docs
> - User signals: popular search results crawled more
> - Sitemap hints: publisher says "this page changes daily"

### Q5: How do you handle content in different formats?

> **Model Answer**:
>
> - Content-Type header → appropriate parser
> - HTML: DOM parser + text extraction
> - PDF: pdf2text libraries
> - Images: OCR for text, visual ML for content
> - Videos: extract metadata, sample frames

---

## 8. Search Engine

### Q1: How do you handle a query with no results?

> **Model Answer**:
>
> - Spell correction: "pythn" → "python"
> - Query expansion: synonyms, stemming
> - Relax matching: AND → OR
> - Show "Did you mean...?" suggestions
> - Fall back to related topics

### Q2: What if one shard is much slower than others?

> **Model Answer**:
>
> - Timeout per shard (e.g., 100ms)
> - Return partial results if some shards timeout
> - Background: investigate slow shard (hot partition? GC?)
> - Rebalance: move hot documents to other shards

### Q3: How do you prevent SEO spam?

> **Model Answer**:
>
> - ML model on content quality
> - Link analysis: penalize unnatural link patterns
> - User signals: low click-through = lower rank
> - Manual actions for egregious cases
> - Regular algorithm updates

### Q4: How do you handle multi-word phrase queries?

> **Model Answer**:
>
> - Inverted index with position data
> - "quick brown fox": find docs with all 3 terms in sequence
> - Intersect posting lists, then filter by position
> - More expensive than single-term: cache phrase results

### Q5: How do you make search feel instant?

> **Model Answer**:
>
> - Query cache: popular queries pre-computed
> - Posting list cache: frequent terms in memory
> - Early termination: after enough good results, stop
> - Tiered index: check high-quality docs first

---

## 9. Notification System

### Q1: How do you prevent notification spam to users?

> **Model Answer**:
>
> - Rate limiting per user per hour (max 10 notifications)
> - Batching: combine multiple events into digest
> - Quiet hours: don't send between 10pm-8am (user timezone)
> - User controls: granular opt-out per category

### Q2: What if APNS or FCM is down?

> **Model Answer**:
>
> - Queue messages for retry
> - Fallback to SMS for critical (OTP, security alerts)
> - In-app notification always stored (user sees on next open)
> - Monitor provider status, alert early

### Q3: How do you handle a marketing campaign to 10M users?

> **Model Answer**:
>
> - Pre-schedule: don't burst all at once
> - Stagger: send over 1-2 hours
> - Segment: first send to 1%, monitor metrics, then full send
> - Backpressure: respect provider rate limits

### Q4: How do you track notification effectiveness?

> **Model Answer**:
>
> - Delivery: confirmed by provider callback
> - Open: tracking pixel in email, callback in push
> - Click: redirect through tracking URL
> - Conversion: attribute downstream action
> - Funnel: Sent → Delivered → Opened → Clicked → Converted

### Q5: How do you personalize notifications?

> **Model Answer**:
>
> - ML model predicts: best time, best channel, best content
> - User history: what do they engage with?
> - A/B test subject lines and content
> - Reinforcement learning: optimize for long-term engagement

---

## Cross-Cutting Questions (Any System)

### Q: How do you handle a database failover?

> - Automatic failover with minimal downtime (< 30 sec)
> - Read replicas continue serving reads
> - Write queue until new primary elected
> - Client library handles redirect to new primary

### Q: What's your monitoring strategy?

> - Four golden signals: latency, traffic, errors, saturation
> - Dashboards per service
> - Alerting with PagerDuty/Opsgenie
> - Distributed tracing (Jaeger/Zipkin)

### Q: How do you do capacity planning?

> - Track growth trends (weekly, monthly)
> - Project forward with safety margin (2x)
> - Load test before major launches
> - Auto-scaling for spiky workloads

### Q: How do you ensure data consistency across services?

> - Saga pattern for distributed transactions
> - Eventual consistency with idempotency
> - Event sourcing for audit trail
> - Compensating transactions for rollback

### Q: What's your deployment strategy?

> - Blue-green or canary deployments
> - Feature flags for gradual rollout
> - Automatic rollback on error rate spike
> - Database migrations separate from code deploys
