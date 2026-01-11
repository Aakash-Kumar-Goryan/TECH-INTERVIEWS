# Security & Privacy

Security is a **non-functional requirement** in every system design. FAANG expects understanding of authentication flows, encryption, and common attack mitigations.

## 1. Authentication vs Authorization

| Concept | Question Answered | Example |
|---------|-------------------|---------|
| **Authentication (AuthN)** | "Who are you?" | Login with username/password |
| **Authorization (AuthZ)** | "What can you do?" | Can this user delete posts? |

---

## 2. OAuth 2.0 (Industry Standard)

OAuth 2.0 is a **delegation protocol**. It allows a third-party app to access resources on behalf of a user without exposing credentials.

### Key Roles

- **Resource Owner**: The user who owns the data.
- **Client**: The application requesting access.
- **Authorization Server**: Issues tokens (e.g., Google, Auth0).
- **Resource Server**: Hosts the protected resources (e.g., API).

### Common Grant Types

| Grant Type | Use Case | Flow |
|------------|----------|------|
| **Authorization Code** | Web apps with backend | User redirected to auth server → Code returned → Backend exchanges code for token |
| **Client Credentials** | Machine-to-machine (no user) | Client sends ID + Secret → Gets token directly |
| **Implicit** (Deprecated) | SPAs (legacy) | Token returned directly in URL (insecure) |
| **PKCE** | Mobile/SPA (modern) | Authorization Code + code_verifier for public clients |

### Authorization Code Flow (Most Common)

```
1. User clicks "Login with Google"
2. Redirect to: https://accounts.google.com/authorize?
     client_id=...&redirect_uri=...&scope=email&response_type=code
3. User authenticates with Google
4. Google redirects to: https://yourapp.com/callback?code=ABC123
5. Your backend exchanges code for tokens:
     POST /token { code=ABC123, client_secret=... }
6. Google returns: { access_token, refresh_token, id_token }
```

---

## 3. JWT (JSON Web Token)

JWTs are self-contained tokens used for stateless authentication.

### Structure: `Header.Payload.Signature`

```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.
eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4ifQ.
SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c
```

| Part | Content | Encoded |
|------|---------|---------|
| **Header** | Algorithm + Type | `{"alg":"HS256","typ":"JWT"}` |
| **Payload** | Claims (user data, expiry) | `{"sub":"123","name":"John","exp":1700000}` |
| **Signature** | HMAC(Header+Payload, secret) | Verifies integrity |

### Token Types

- **Access Token**: Short-lived (15 min). Sent with each API request.
- **Refresh Token**: Long-lived (days/weeks). Used to get new access tokens.

### Security Considerations

- Never store JWTs in `localStorage` (XSS vulnerable). Use `httpOnly` cookies.
- Always validate signature and expiry (`exp` claim).
- Include `aud` (audience) claim to prevent token misuse.

---

## 4. Authorization Models

### RBAC (Role-Based Access Control)

Permissions assigned to **roles**, users assigned to roles.

```
Roles: Admin, Editor, Viewer
User "Alice" → Role "Editor" → Permissions: [read, write]
```

- **Pros**: Simple, widely understood.
- **Cons**: Role explosion in complex systems.

### ABAC (Attribute-Based Access Control)

Permissions based on **attributes** (user, resource, environment).

```
Rule: Allow if user.department == resource.department AND time < 6pm
```

- **Pros**: Fine-grained, flexible.
- **Cons**: Complex policy management.

---

## 5. Encryption

### At Rest vs In Transit

| Type | Purpose | Technology |
|------|---------|------------|
| **At Rest** | Protect stored data | AES-256, AWS KMS, disk encryption |
| **In Transit** | Protect data during transfer | TLS/SSL (HTTPS) |

### Symmetric vs Asymmetric

| Type | Keys | Speed | Use Case |
|------|------|-------|----------|
| **Symmetric** | Same key encrypts/decrypts | Fast | Bulk data encryption (AES) |
| **Asymmetric** | Public + Private key pair | Slow | Key exchange, digital signatures (RSA) |

### TLS 1.3 Handshake (Simplified)

1. **Client Hello**: Supported cipher suites, random number.
2. **Server Hello**: Chosen cipher, server certificate (public key).
3. **Key Exchange**: Client generates session key, encrypts with server's public key.
4. **Secure Channel**: Both sides use session key for symmetric encryption.

---

## 6. API Security Best Practices

### Common Attacks & Mitigations

| Attack | Description | Mitigation |
|--------|-------------|------------|
| **XSS** | Inject malicious scripts | Sanitize input, Content-Security-Policy |
| **CSRF** | Trick user into unintended actions | CSRF tokens, SameSite cookies |
| **SQL Injection** | Inject SQL via input | Parameterized queries, ORM |
| **DDoS** | Overwhelm with traffic | Rate limiting, CDN, WAF |

### Headers to Set

```http
Content-Security-Policy: default-src 'self'
X-Content-Type-Options: nosniff
X-Frame-Options: DENY
Strict-Transport-Security: max-age=31536000; includeSubDomains
```

### Input Validation

- **Whitelist** allowed characters, not blacklist.
- Validate on **server-side** (client validation is bypassable).
- Use schema validation (JSON Schema, Protobuf).

---

## 7. Rate Limiting for Security

Distinguish between **abuse** and **legitimate bursts**.

| Strategy | Description |
|----------|-------------|
| **Per-IP** | Limit requests from single IP |
| **Per-User** | Limit authenticated user's requests |
| **Per-API-Key** | Limit by API key (for B2B) |
| **Adaptive** | Increase limits for trusted users |

Return `429 Too Many Requests` with `Retry-After` header.

---

## 8. Compliance Basics (Awareness Level)

| Regulation | Scope | Key Requirement |
|------------|-------|-----------------|
| **GDPR** | EU user data | Right to deletion, consent |
| **HIPAA** | US healthcare | Encryption, audit logs |
| **PCI-DSS** | Payment data | Tokenization, secure storage |
| **SOC 2** | Service orgs | Security controls audit |
