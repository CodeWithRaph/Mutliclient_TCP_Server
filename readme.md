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

## Multiclient

Clients connect one by one to the main socket.<br>
This socket listens for incoming client/server connections, then redirects each client to a dedicated thread.<br>
This allows each client to maintain its own individual socket connection.<br>
As a result, there are no inter-client conflicts with this approach.<br>