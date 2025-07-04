#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

/*
1. Create server socket
2. Bind socket to an address and port
3. Listen for incoming connections
4. Accept a connection from a client
5. Receive a message from the client
6. Send a reply to the client
7. Close the sockets
*/

#define PORT 12345
#define MAX_LEN 100

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[MAX_LEN] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on any network interface
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }

    listen(server_fd, 1);
    std::cout << "Server listening on port " << PORT << "...\n";

    socklen_t addrlen = sizeof(address);
    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_fd < 0) {
        std::cerr << "Accept failed\n";
        close(server_fd);
        return 1;
    }


    int valread = recv(client_fd, buffer, MAX_LEN, 0);
    if (valread > 0) {
        std::cout << "Client: " << buffer << std::endl;
    }


    std::string reply = "Hello from server!";
    send(client_fd, reply.c_str(), reply.length() + 1, 0);


    close(client_fd);
    close(server_fd);
    std::cout << "Connection closed.\n";
    return 0;
}