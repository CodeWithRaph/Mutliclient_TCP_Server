# Context

This projet is part of a **Proof of Concept** *(POC)* for a larger private project.<br>

As a result, the code is commented in French — apologies for that.<br>
It is also structured to support automatic documentation generation using Doxygen.

# Multiclient TCP Server

I encountered several issues when transitioning from a single‑client TCP server to a multi‑client TCP server.<br>
Sharing the same server instance can lead to socket concurrency problems.<br>
Let’s take a look at the structural differences to better understand this behavior.<br>

## Single client

The client connects to the socket.<br>
It can send messages as long as the socket remains open.<br>
A limitation of this architecture is that issues arise when attempting to connect multiple clients simultaneously.<br>

```mermaid
---
Title: TCP Client example
---
flowchart LR


subgraph TCP Client
    Config["`IP: 192.168.0.10
    `"]

    Connect(["client(#quot;192.168.0.50#quot;, 8080);"])

    TCP["TCP exchanges successfull"]
end

subgraph TCP Client 2
    Config2["`IP: 192.168.0.11
    `"]

    Connect2(["client(#quot;192.168.0.50#quot;, 8080);"])

    TCP2["TCP exchanges successfull"]
end

subgraph TCP Server
    Config_Server["`IP: 192.168.0.50
    Port: 8080
    `"]

    Socket
end

Connect <--> Socket
Socket <--> Connect2


TCP <==>
Socket x==x TCP2

```

## Multiclient

Clients connect one by one to the main socket.<br>
This socket listens for incoming client/server connections, then redirects each client to a dedicated thread.<br>
This allows each client to maintain its own individual socket connection.<br>
As a result, there are no inter-client conflicts with this approach.<br>

```mermaid
---
Title: TCP Client example
---
flowchart LR


subgraph TCP Client
    direction TB
    Config["`IP: 192.168.0.10
    `"]

    Connect(["client(#quot;192.168.0.50#quot;, 8080);"])

    TCP["TCP exchanges successfull"]
end

subgraph TCP Client 2
    direction TB
    Config2["`IP: 192.168.0.11
    `"]

    Connect2(["client(#quot;192.168.0.50#quot;, 8080);"])

    TCP2["TCP exchanges failed"]
end

subgraph TCP Server
    direction TB
    Config_Server["`IP: 192.168.0.50
    Port: 8080
    `"]
    subgraph Main Socket
        Socket["Socket"]
        Queue["`Queue:
        Client 1
        Client 2
        `"]
    end

    subgraph Threads
        direction TB
        subgraph Thread 1
            Socket1["Socket 1"]
        end

        subgraph Thread 2
            Socket2["Socket 2"]
        end
    end
end

Connect -- Client 1 | Step 1 --> Socket
Connect2 -- Client 2 | Step 1 --> Socket

Socket -- Client 1 | Step 2 --> Socket1
Socket -- Client 2 | Step 2 --> Socket2

TCP <== Client 1 | Step 3 ==> Socket1
TCP2 <== Client 2 | Step 3 ==> Socket2 

```