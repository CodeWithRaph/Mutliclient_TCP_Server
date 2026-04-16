#include "server_tcp.hpp"

int main() {
    TCPServer server(8080);
    server.run();
}