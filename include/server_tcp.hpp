#ifndef SERVER_TCP_HPP
#define SERVER_TCP_HPP

/**
 * @brief Serveur TCP simple multi-client
 */
class TCPServer {
private:
    int server_fd;
    int client_socket;
    struct sockaddr_in address;
    int addrlen;

public:
    /**
     * @brief Constructeur
     * @param port Port d'écoute
     */
    TCPServer(int port);

    /**
     * @brief Gère la réception isolée des messages d'un client.
     */
    void receiveLoop(int client_socket, sockaddr_in client_addr);

    /**
     * @brief Lance la boucle d'acceptation des clients
     * Lorsqu'un client tente d'ouvrir une connexion.
     * Cette boucle l'accepte automatiquement, puis l'isole dans un thread dédié.
     */
    void run();
};

#endif