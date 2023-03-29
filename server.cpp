#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {

    char buffer[MAX_BUFFER_SIZE];

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);

    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    int bindStatus = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if(bindStatus < 0) {
        std::cerr<<"Failed to bind socket to port "<< port <<std::endl;
        return 1;
    }

    if(listen(serverSocket, 5) < 0) {
        std::cerr<<"Failed to listen on port "<< port <<std::endl;
        return 1;
    }
    std::cout<<"TCP server listening on port "<< port <<std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        std::cout << "Accepted connection from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;

        size_t bytesReceived;
        while ((bytesReceived = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Received data from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << ": " << buffer << std::endl;

        }
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}