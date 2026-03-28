#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>

#ifdef _WIN32
    // Windows sockets
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")  // link Winsock library
#else
    // Unix / macOS sockets
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

#endif // PROTOCOL_H

enum Opcode : uint8_t {
    OP_OPEN_ACCOUNT = 1,
    OP_CLOSE_ACCOUNT = 2,
    OP_DEPOSIT_WITHDRAW = 3,
    OP_CHECK_BALANCE = 4,   
    OP_TRANSFER = 5,        
    OP_MONITOR_UPDATES = 6,
    OP_REPLY = 7,
    OP_ERROR = 8,
    OP_CALLBACK_UPDATE = 9
};

enum Currency : uint8_t {
    CUR_SGD = 1,
    CUR_USD = 2,
    CUR_EUR = 3
};

enum Semantics : uint8_t {
    AT_LEAST_ONCE = 0,
    AT_MOST_ONCE = 1
};

struct MessageHeader {
    uint32_t request_id;
    uint8_t opcode;
    uint8_t semantics;
    uint32_t payload_length;

    void marshal(std::vector<uint8_t>& buffer) const {
        uint32_t req_net = htonl(request_id);
        uint32_t len_net = htonl(payload_length);
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&req_net), reinterpret_cast<uint8_t*>(&req_net) + 4);
        buffer.push_back(opcode);
        buffer.push_back(semantics);
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&len_net), reinterpret_cast<uint8_t*>(&len_net) + 4);
    }

    static MessageHeader unmarshal(const std::vector<uint8_t>& buffer, size_t& offset) {
        MessageHeader header;
        uint32_t req_net, len_net;
        std::memcpy(&req_net, &buffer[offset], 4); offset += 4;
        header.request_id = ntohl(req_net);
        header.opcode = buffer[offset++];
        header.semantics = buffer[offset++];
        std::memcpy(&len_net, &buffer[offset], 4); offset += 4;
        header.payload_length = ntohl(len_net);
        return header;
    }
};

class Marshaller {
    std::vector<uint8_t> buffer;
public:
    void append_int(int32_t val) {
        uint32_t net_val = htonl(static_cast<uint32_t>(val));
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&net_val), reinterpret_cast<uint8_t*>(&net_val) + 4);
    }
    void append_float(float val) {
        uint32_t net_val;
        std::memcpy(&net_val, &val, 4);
        net_val = htonl(net_val);
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&net_val), reinterpret_cast<uint8_t*>(&net_val) + 4);
    }
    void append_string(const std::string& str) {
        uint16_t len = htons(static_cast<uint16_t>(str.length()));
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&len), reinterpret_cast<uint8_t*>(&len) + 2);
        buffer.insert(buffer.end(), str.begin(), str.end());
    }
    void append_byte(uint8_t byte) {
        buffer.push_back(byte);
    }
    const std::vector<uint8_t>& get_buffer() const { return buffer; }
    size_t get_size() const { return buffer.size(); }
};

class Unmarshaller {
    const std::vector<uint8_t>& buffer;
    size_t offset;
public:
    Unmarshaller(const std::vector<uint8_t>& buf, size_t start_offset = 0) : buffer(buf), offset(start_offset) {}
    int32_t read_int() {
        if (offset + 4 > buffer.size()) return 0;
        uint32_t net_val;
        std::memcpy(&net_val, &buffer[offset], 4);
        offset += 4;
        return static_cast<int32_t>(ntohl(net_val));
    }
    float read_float() {
        if (offset + 4 > buffer.size()) return 0.0f;
        uint32_t net_val;
        std::memcpy(&net_val, &buffer[offset], 4);
        offset += 4;
        net_val = ntohl(net_val);
        float val;
        std::memcpy(&val, &net_val, 4);
        return val;
    }
    std::string read_string() {
        if (offset + 2 > buffer.size()) return "";
        uint16_t net_len;
        std::memcpy(&net_len, &buffer[offset], 2);
        offset += 2;
        uint16_t len = ntohs(net_len);
        if (offset + len > buffer.size()) return "";
        std::string str(reinterpret_cast<const char*>(&buffer[offset]), len);
        offset += len;
        return str;
    }
    uint8_t read_byte() {
        if (offset + 1 > buffer.size()) return 0;
        return buffer[offset++];
    }
    bool is_empty() const { return offset >= buffer.size(); }
};

