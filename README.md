# C++ Raw Socket Packet Sniffer

A lightweight, high-performance Layer 2–Layer 4 network packet sniffer written in modern C++ utilizing Linux raw sockets (`AF_PACKET`, `SOCK_RAW`). Designed with memory bounds safety, dynamic header parsing, bitfield flag decoding, and JSON telemetry emission for downstream SIEM/SOC ingestion pipelines.

---

## Features

- **Direct Kernel-to-NIC Interfacing:** Uses `AF_PACKET` with `ETH_P_ALL` to bypass standard OS network stack filtering and read incoming/outgoing Ethernet frames directly.
- **Defensive Memory & Bounds Checking:** Enforces explicit `ssize_t` boundary checks across Ethernet, IPv4, and Transport layers to defend against malformed frames and buffer over-reads.
- **Dynamic Header Parsing:** Calculates dynamic IP Header Length (`ihl * 4`) to accurately offset into Layer 4 data, even when IP options are present.
- **Protocol Demuxing:**
  - **TCP:** Port extraction via network-to-host byte conversion (`ntohs`) and bitfield accumulation for active control flags (`SYN`, `ACK`, `FIN`, `PSH`, `URG`, `RST`).
  - **UDP:** Port extraction and stream telemetry.
  - **ICMP:** Direct 8-bit extraction for ICMP Type and Code (ping requests/replies, unreachable destinations) without erroneous byte swapping.
- **Thread-Safe Addressing:** Uses `inet_ntop` with dedicated string buffers to eliminate static-buffer overwrite issues common to legacy `inet_ntoa`.
- **Telemetry Pipeline Ready:** Features built-in ANSI-colored live terminal logging and an integrated `tojson()` serializer for streaming parsed events into IPC pipes, Python ingestion workers, or SOC dashboards.

---

## Architecture Overview

[ Ethernet Frame (Layer 2) ] ──► 14-byte ethhdr offset
│
▼
[ IPv4 Header (Layer 3) ]    ──► Boundary check & dynamic (ihl * 4) calculation
│
┌─────┴───────────────┬────────────────────────┐
▼                     ▼                        ▼
[ TCP Header (L4) ]   [ UDP Header (L4) ]      [ ICMP Header (L4) ]

    Ports (ntohs)       - Ports (ntohs)          - Type & Code (8-bit direct)

    Flags (Bitfields)   - Packet Size            - Port normalization (0)
    │                     │                        │
    └─────────────────────┼────────────────────────┘
    ▼
    [ struct packet_info ]
    │
    ┌───────────┴───────────┐
    ▼                       ▼
    Terminal Display            JSON Serialization
    (ANSI Color Coded)          (tojson() for IPC)


---

## File Structure

- `headers.hpp`: Data structures (`packet_info`, `snifferconfig`), protocol headers, and serialization logic.
- `sniffer_core.cpp`: Socket initialization, frame capture loop, pointer arithmetic, protocol branching, and output formatting.

---

## Prerequisites & Dependencies

- **OS:** Linux (Kernel 3.x+ recommended)
- **Compiler:** `g++` (C++11 standard or higher)
- **Privileges:** Root access (`sudo`) or `CAP_NET_RAW` Linux capability to bind raw sockets.

---

## Build Instructions

Compile with optimizations and strict warning flags:

```bash
g++ -O2 -Wall -Wextra -std=c++17 sniffer_core.cpp -o sniffer


## Running Packet Sniffer

#1> with sudo 
 
sudo ./sniffer

#2> without sudo(for security)
sudo setcap cap_net_raw,cap_net_admin=eip ./sniffer
./sniffer

---


##Testing -->

# 1. Test ICMP parsing (Type 8 echo request, Type 0 reply)
ping -c 2 1.1.1.1

# 2. Test UDP parsing (DNS query over port 53)
dig google.com

# 3. Test TCP parsing & flag decoding (SYN / ACK / PSH)
curl -I [http://example.com](http://example.com)

