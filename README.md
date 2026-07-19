# Tiny HTTP Server

A modular multithreaded HTTP server built from scratch in **C++17** using **POSIX sockets** and the Linux socket API.

This project was developed to strengthen my understanding of operating systems, computer networks, concurrent programming, and systems software by implementing the HTTP request lifecycle without relying on existing web frameworks.

---

## Features

- TCP socket programming using POSIX sockets
- HTTP/1.1 request parsing
- HTTP response generation
- Static HTML file serving
- Request routing
- MIME type detection
- Multithreaded request handling using `std::thread`
- Thread-safe logging using `std::mutex`
- Live server statistics endpoint (`/stats`)
- Graceful shutdown support
- Modular object-oriented architecture

---

## Technologies

- C++17
- POSIX Sockets
- Linux / Ubuntu (WSL)
- Make
- Git

---

## Project Structure

```
tiny-http-server/
│
├── public/
│   ├── index.html
│   ├── about.html
│   └── 404.html
│
├── src/
│   ├── server.cpp
│   ├── request.cpp
│   ├── response.cpp
│   ├── router.cpp
│   ├── mime.cpp
│   ├── logger.cpp
│   ├── stats.cpp
│   └── ...
│
├── tests/
├── Makefile
└── README.md
```

---

## Architecture

```
Browser / curl
        │
        ▼
TCP Socket
        │
        ▼
Server
        │
        ▼
Request Parser
        │
        ▼
Router
        │
        ├──────────────┐
        ▼              ▼
 Static Files      /stats API
        │              │
        └──────┬───────┘
               ▼
      Response Builder
               │
               ▼
          HTTP Response
```

---

## HTTP Request Flow

```
Client

↓

socket()

↓

accept()

↓

recv()

↓

HttpRequest

↓

Router

↓

Static File / JSON

↓

HttpResponse

↓

send()

↓

close()
```

---

## Example Endpoints

### Home

```
GET /
```

Returns

```
public/index.html
```

---

### About

```
GET /about
```

Returns

```
public/about.html
```

---

### Statistics

```
GET /stats
```

Example response

```json
{
  "uptimeSeconds": 143,
  "requestsServed": 57,
  "activeConnections": 2
}
```

---

### Invalid Route

```
GET /random
```

Returns

```
404 Not Found
```

---

## Build

```bash
make
```

Run

```bash
./server
```

---

## Concepts Demonstrated

- Socket Programming
- Operating Systems
- HTTP Protocol
- TCP Networking
- Object-Oriented Design
- Concurrent Programming
- Thread Synchronization
- Atomic Operations
- Systems Programming
- Linux Development


**Nehaa J**

Computer Science Engineering Student

PES University
