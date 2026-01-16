# ML/NLP System Design

> **For Google L5**: ML-based system design questions are increasingly common. Focus on end-to-end pipeline thinking, not just model architecture.

---

## ML System Components Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                        ML System Architecture                        │
├─────────────────────────────────────────────────────────────────────┤
│                                                                      │
│   Data Sources ──▶ Feature Store ──▶ Model Training ──▶ Model Store │
│        │                 │                                    │     │
│        │                 ▼                                    ▼     │
│        └────────▶ Feature Serving ──▶ Model Serving ──▶ Predictions │
│                          │                    │                      │
│                          └────────────────────┴──▶ Logging/Metrics  │
│                                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

---

## Key Components

### 1. Feature Store

Central repository for feature computation and serving.

| Aspect | Offline (Training) | Online (Inference) |
|--------|-------------------|-------------------|
| Latency | Minutes/Hours | < 10ms |
| Freshness | Batch | Real-time |
| Storage | Data warehouse | Redis/Memcached |
| Scale | Petabytes | Millions QPS |

**Design Considerations**:

- Feature consistency between training and serving (training-serving skew)
- Point-in-time correctness for historical features
- Feature versioning and lineage tracking

### 2. Model Serving

| Pattern | Use Case | Latency | Throughput |
|---------|----------|---------|------------|
| **Direct serving** | Simple models, low traffic | Low | Low |
| **Model as microservice** | Independent scaling | Medium | Medium |
| **Batched inference** | High throughput needs | Higher | High |
| **Edge deployment** | Mobile, IoT | Lowest | Limited |

**Optimization Techniques**:

- Model quantization (FP32 → INT8)
- Knowledge distillation (large → small model)
- Batching requests for GPU efficiency
- Caching common predictions

### 3. A/B Testing & Experimentation

```
Traffic Split:
├── Control (50%) ──▶ Model v1
└── Treatment (50%) ──▶ Model v2
         │
         ▼
    Metrics Collection
         │
         ▼
    Statistical Analysis
```

---

## Design Pattern 1: Search Ranking System

### Components

```
User Query
    │
    ▼
┌──────────────────┐
│ Query Processing │ ──▶ Tokenization, spell check, query expansion
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│    Retrieval     │ ──▶ Inverted index, approximate nearest neighbor
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│     Ranking      │ ──▶ L1: Simple model (fast), L2: Complex model (accurate)
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│  Personalization │ ──▶ User history, context, location
└────────┬─────────┘
         │
         ▼
   Ranked Results
```

### L1/L2 Ranking Architecture

| Stage | Model | Candidates | Latency Budget |
|-------|-------|------------|----------------|
| Retrieval | ANN/BM25 | Millions → 10K | 50ms |
| L1 Ranking | Linear/Tree | 10K → 500 | 20ms |
| L2 Ranking | Neural Network | 500 → 50 | 30ms |

### Key Features for Ranking

- **Query features**: length, terms, intent classification
- **Document features**: quality score, freshness, popularity
- **Query-document features**: TF-IDF, BM25, semantic similarity
- **User features**: history, preferences, location

---

## Design Pattern 2: Recommendation Engine

### Approaches

| Approach | Pros | Cons |
|----------|------|------|
| **Collaborative Filtering** | No content analysis needed | Cold start problem |
| **Content-Based** | No cold start for items | Limited discovery |
| **Hybrid** | Best of both | More complex |
| **Deep Learning** | Captures complex patterns | Computationally expensive |

### Architecture

```
┌────────────────────────────────────────────────────────┐
│                 Recommendation System                   │
├────────────────────────────────────────────────────────┤
│                                                         │
│  Offline Pipeline (Daily/Hourly)                       │
│  ├── User embeddings computation                       │
│  ├── Item embeddings computation                       │
│  └── Candidate generation (ANN index)                  │
│                                                         │
│  Online Pipeline (Real-time)                           │
│  ├── Retrieve user embedding                           │
│  ├── Query ANN index for candidates (~1000)            │
│  ├── Fetch real-time features                          │
│  ├── Score and rank candidates                         │
│  └── Apply business rules & diversification            │
│                                                         │
└────────────────────────────────────────────────────────┘
```

### Handling Cold Start

| Scenario | Solution |
|----------|----------|
| New user | Popular items, demographic-based |
| New item | Content-based until interactions |
| New user + item | Explore/exploit with bandits |

---

## Design Pattern 3: Real-time Classification (Spam/Toxicity)

### Requirements

- **Latency**: < 50ms per request
- **Throughput**: 100K+ QPS
- **Accuracy**: High precision (minimize false positives)

### Architecture

```
Input Text
    │
    ▼
┌──────────────────┐
│   Preprocessing  │ ──▶ Normalization, language detection
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│   Fast Filter    │ ──▶ Blocklist, regex patterns (< 1ms)
└────────┬─────────┘
         │ (passed)
         ▼
┌──────────────────┐
│    ML Model      │ ──▶ BERT/DistilBERT classification
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│  Post-processing │ ──▶ Threshold, confidence calibration
└────────┬─────────┘
         │
         ▼
   Classification Result + Confidence Score
```

### Optimization for Low Latency

- **Model distillation**: Use smaller, faster models
- **Caching**: Cache predictions for repeated content
- **Batching**: Group requests for GPU efficiency
- **Early exit**: Skip ML if blocklist matches

---

## NLP-Specific Components

### Tokenization Service

```
Text ──▶ Tokenizer ──▶ Token IDs ──▶ Model
              │
              ▼
         Vocabulary (stored/versioned)
```

**Considerations**:

- Keep tokenizer in sync with model version
- Handle unknown tokens gracefully
- Support multiple languages

### Embedding Service

| Use Case | Approach |
|----------|----------|
| Static embeddings | Pre-compute, store in vector DB |
| Dynamic embeddings | Compute on-the-fly |
| Caching strategy | LRU cache for frequent queries |

**Scale**:

- Store billions of embeddings
- Serve at < 5ms latency
- Use ANN (Approximate Nearest Neighbor) for similarity search

---

## Scalability Patterns

### Model Sharding

For very large models:

```
Model (100GB)
├── Shard 1 (25GB) ──▶ GPU 1
├── Shard 2 (25GB) ──▶ GPU 2  
├── Shard 3 (25GB) ──▶ GPU 3
└── Shard 4 (25GB) ──▶ GPU 4
         │
         ▼
    Aggregation Layer
```

### Request Batching

```
Incoming Requests
    │
    ▼
┌──────────────────┐
│   Batch Buffer   │ ──▶ Collect for 5-10ms OR until batch size hit
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│  Batched Inference│ ──▶ GPU processes batch efficiently
└────────┬─────────┘
         │
         ▼
    Route results back to individual requests
```

---

## Metrics: Model vs System

| Type | Metrics |
|------|---------|
| **Model Quality** | Precision, Recall, AUC, NDCG |
| **System Performance** | Latency (P50/P99), Throughput, Error rate |
| **Business Impact** | CTR, Revenue, User engagement |

**Key Insight**: A 1% improvement in model accuracy may not matter if P99 latency doubles.

---

## Interview Tips for ML System Design

1. **Clarify the ML task**: Is it classification, ranking, generation?
2. **Define success metrics**: Both model and business metrics
3. **Design end-to-end**: Data → Training → Serving → Monitoring
4. **Address latency/throughput trade-offs** early
5. **Discuss training-serving skew** prevention
6. **Include feedback loop**: How does the model improve over time?

---

## Practice Questions

| Question | Key Focus |
|----------|-----------|
| Design YouTube Recommendations | Two-tower model, candidate generation |
| Design Search Ranking | L1/L2 ranking, feature engineering |
| Design Spam Detection | Real-time inference, high precision |
| Design Ad Targeting | User segmentation, real-time bidding |
| Design Chatbot/Assistant | NLU pipeline, intent classification |
| Design Google Translate | Encoder-decoder, batching, caching |
