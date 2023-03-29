#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {

    char buffer[MAX_BUFFER_SIZE];

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }

    const char* ip = argv[1];
    int port = std::atoi(argv[2]);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddress;

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid IP address" << std::endl;
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    std::string message;

    while (getline(std::cin, message)) {
        if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
            std::cerr << "Failed to send data" << std::endl;
            return 1;
        }

    }

    close(clientSocket);
    return 0;
}
