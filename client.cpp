#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
using namespace std;

/*
Multi-threaded Chatroom Client:
1. Create socket
2. Connect to server 
3. Start two threads:
   - One for sending messages (user input)
   - One for receiving messages (from other clients)
4. Handle real-time communication
5. Graceful disconnection
*/

#define PORT 12345
#define MAX_LEN 1024

bool running = true;

// Function to continuously receive messages from server
void receiveMessages(int sock) {
    char buffer[MAX_LEN];
    
    while (running) {
        memset(buffer, 0, MAX_LEN);
        int valread = recv(sock, buffer, MAX_LEN, 0);
        
        if (valread <= 0) {
            cout << "\n*** Disconnected from server ***" << endl;
            running = false;
            break;
        }
        
        cout << buffer;
        cout.flush(); // Ensure immediate output
    }
}

// Function to continuously send messages to server
void sendMessages(int sock) {
    string message;
    
    while (running) {
        getline(cin, message);
        
        if (message == "quit") {
            running = false;
            break;
        }
        
        if (!message.empty()) {
            send(sock, message.c_str(), message.length(), 0);
        }
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket creation error\n";
        return 1;
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported\n";
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection failed\n";
        return 1;
    }

    cout << "Connected to chatroom server!" << endl;

    // Handle username prompt from server
    char buffer[MAX_LEN];
    int valread = recv(sock, buffer, MAX_LEN, 0);
    if (valread > 0) {
        cout << buffer;
        cout.flush();
    }
    
    // Get username from user
    string username;
    getline(cin, username);
    send(sock, username.c_str(), username.length(), 0);
    
    cout << "\n=== Chatroom ===" << endl;
    cout << "Type your messages and press Enter to send." << endl;
    cout << "Type 'quit' to exit the chatroom." << endl;
    cout << "=================" << endl;

    // Create two threads: one for receiving, one for sending
    thread receiver(receiveMessages, sock);
    thread sender(sendMessages, sock);

    // Wait for both threads to complete
    sender.join();  // Wait for sender thread (user input)
    running = false; // Signal receiver thread to stop
    
    // Close socket to break receiver thread's recv() call
    close(sock);
    
    if (receiver.joinable()) {
        receiver.join(); // Wait for receiver thread
    }

    cout << "*** Goodbye! ***" << endl;
    return 0;
}