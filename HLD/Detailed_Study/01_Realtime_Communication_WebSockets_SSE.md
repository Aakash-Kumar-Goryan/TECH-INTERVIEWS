# Real-time Communication: WebSockets & Server-Sent Events (SSE)

Modern applications often require real-time updates (e.g., chat, live scores, collaborative editing). Standard HTTP request-response models are inefficient for this.

## 1. WebSockets

### Overview

WebSockets provide a **full-duplex (bi-directional)** communication channel over a single, long-lived TCP connection. It allows both the client and the server to send data to each other independently at any time.

* **Protocol**: Starts as an HTTP request (Handshake) and upgrades to the WebSocket protocol (`ws://` or `wss://`).
* **Persistent Connection**: The connection stays open until explicitly closed by the client or server.

### How it Works (The Handshake)

1. **Client** sends a standard HTTP GET request with an `Upgrade: websocket` header.
2. **Server** validates the request and responds with `101 Switching Protocols`.
3. The connection is now a WebSocket connection (TCP/IP). Data frames can flow in both directions.

### Pros

* **Real-time Bi-directional**: Essential for apps where the client also sends frequent updates (e.g., Multiplayer games, Collaborative docs).
* **Low Overhead**: After the handshake, data frames have very small headers compared to HTTP.
* **Efficient**: No need to establish a new connection for every message.

### Cons

* **Complexity**: Requires handling connection state on the server (stateful).
* **Firewalls/Proxies**: Some corporate firewalls block non-standard ports or long-lived connections (though `wss://` over port 443 helps).
* **Resource Heavy**: Maintaining thousands of open connections requires significant server memory and file descriptors.

### Use Cases

* Chat Applications (WhatsApp, Slack)
* Multiplayer Gaming
* Collaborative Editing (Google Docs)
* Financial Trading Platforms

---

## 2. Server-Sent Events (SSE)

### Overview

SSE allows a server to push updates to the client over a standard HTTP connection. It is **mono-directional** (Server -> Client only).

* **Protocol**: Standard HTTP. The client opens a connection, and the server keeps it open, sending data as a stream of text (`text/event-stream`).
* **Browser Support**: Native support in modern browsers via the `EventSource` API.

### How it Works

1. **Client** sends a standard HTTP request.
2. **Server** responds with `Content-Type: text/event-stream` and keeps the connection open.
3. **Server** sends events in a specific text format:

    ```text
    data: {"score": 10, "time": "10:00"}
    
    data: {"score": 12, "time": "10:05"}
    ```

### Pros (SSE)

* **Simpler than WebSockets**: works over standard HTTP, no protocol upgrade needed.
* **Auto-reconnection**: Built-in generic browser support handles dropped connections automatically.
* **Firewall Friendly**: Works effortlessly over standard HTTP ports.
* **Polyfills**: Easy to back-fill for older browsers.

### Cons (SSE)

* **Uni-directional**: Client cannot send data back over the same connection (must verify via separate HTTP POST/GET).
* **Text-only**: Restricted to UTF-8 text (though you can encode binary as Base64).
* **Connection Limits**: Browsers (specifically HTTP/1.x) have a low limit on simultaneous connections per domain (~6). HTTP/2 solves this.

### Use Cases (SSE)

* Live Sports Scores
* Stock Tickers (Read-only)
* Social Media Feeds / Notifications
* CI/CD Build Status Logs

---

## 3. Comparison Summary

| Feature | HTTP Polling | WebSockets | Server-Sent Events (SSE) |
| :--- | :--- | :--- | :--- |
| **Direction** | Client tries, Server responds | Bi-directional | Server -> Client |
| **Connection** | Short-lived | Persistent (Stateful) | Persistent (Stateful) |
| **Protocol** | HTTP | WebSocket (TCP) | HTTP |
| **Efficiency** | Low (Header overhead) | High | High |
| **Complexity** | Low | High | Medium |
| **Use Case** | Dashboard updates | Chat, Gaming | News feed, Notifications |
