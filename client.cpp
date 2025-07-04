#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

/*
1. Create socket
2. Convert IPv4 address from text to binary form 
Connects to localhost (127.0.0.1)
3. Connect to server
4. Send a message to the server
5. Receive reply from server
6. Close socket
*/

#define PORT 12345
#define MAX_LEN 100

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX_LEN] = {0};

    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket creation error\n";
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);


    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported\n";
        return 1;
    }


    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection failed\n";
        return 1;
    }


    cout << "Enter message to send: ";
    cin.getline(buffer, MAX_LEN);
    send(sock, buffer, strlen(buffer) + 1, 0);


    char reply[MAX_LEN] = {0};
    int valread = recv(sock, reply, MAX_LEN, 0);
    if (valread > 0) {
        cout << "Server: " << reply << std::endl;
    }


    close(sock);
    return 0;
}