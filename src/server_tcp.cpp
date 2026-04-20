#include "server_tcp.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

TCPServer::TCPServer(int port) {
    addrlen = sizeof(address);

    // Création du socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configuration
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Attribution d'adresse IP et du port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Attribution échouée");
        exit(1);
    }

    // Ecoute sur le port
    int ret;
    do {
        ret = listen(server_fd, 3);

        if (ret < 0) {
            perror("Ecoute échouée, nouvelle tentative...");
            sleep(1);
        }

    } while (ret < 0);

    std::cout << "Serveur en attente sur le port " << port << "..." << std::endl;
}

void TCPServer::receiveLoop(int client_socket, sockaddr_in client_addr) {
    char buffer[1024];

    while (true) {
        int n = read(client_socket, buffer, 1024);

        if (n == 0) {
            std::cout << "Client " << inet_ntoa(client_addr.sin_addr) << " déconnecté !" << std::endl;
            break;
        }

        if (n < 0) {
            std::cerr << "Erreur read()" << std::endl;
            break;
        }

        buffer[n] = '\0';
        std::cout << "Message reçu : " << buffer << std::endl;

        // Réponse "ok" au client
        const char* ok = "ok\n";
        if (send(client_socket, ok, 3, 0) < 0) {
            std::cerr << "Erreur send()" << std::endl;
            break;
        }
    }

    close(client_socket);
}

void TCPServer::run() {
    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);

        int client_socket = accept(server_fd,
                                (struct sockaddr*)&client_addr,
                                &len);

        std::cout << "Client: "
                << inet_ntoa(client_addr.sin_addr)
                << std::endl;

        std::thread(&TCPServer::receiveLoop,
                    this,
                    client_socket,
                    client_addr).detach();
    }
}