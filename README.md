# Distributed Banking System

## 🛠️ Build & Run Server (Go)

```bash
cd server
go mod tidy
go build -o bank_server main.go

# Usage: ./bank_server <port> [drop_rate]
# Example (30% packet drop rate):
./bank_server 2222 0.3
```

## 🛠️ Build & Run Client (C++)

```bash
cd client
mkdir build && cd build
cmake ..
make

# Usage: ./client_app <Server_IP> <Port> <Semantics: 0=AtLeastOnce, 1=AtMostOnce> [drop_rate]
# Example (At-Most-Once semantics, 30% drop rate):
./client_app 127.0.0.1 2222 1 0.3
```

## 💻 Running the Demo (Locally)

1. **Terminal 1 (Server):** 
   ```bash
   cd server
   ./bank_server 2222 0.3
   ```
2. **Terminal 2 (Monitor Client):**
   ```bash
   cd client/build
   ./client_app 127.0.0.1 2222 1 0.0
   ```
   *Select Option 6 (Monitor Updates)*

3. **Terminal 3 (Operator Client):**
   ```bash
   cd client/build
   ./client_app 127.0.0.1 2222 1 0.3
   ```
   *Perform operations and watch Server and Monitor terminals output live updates.*
