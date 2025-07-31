#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex> 
#include <algorithm>
#include <map>
using namespace std;

/*
Multi-client Chatroom Server:
1. Create server socket
2. Bind socket to an address and port
3. Listen for incoming connections
4. Accept multiple client connections
5. Handle each client in a separate thread
6. Broadcast messages from one client to all others
7. Handle client disconnections gracefully
*/

#define PORT 12345
#define MAX_LEN 1024
#define MAX_CLIENTS 10

// Global variables for client management
vector<int> clients;
map<int, string> client_usernames; // Map client_fd to username
mutex clients_mutex;

// Function to broadcast message to all clients except sender
void broadcastMessage(const string& message, int sender_fd) {
    lock_guard<mutex> lock(clients_mutex);
    
    for (int client_fd : clients) {
        if (client_fd != sender_fd) {
            send(client_fd, message.c_str(), message.length(), 0);
        }
    }
}

// Function to remove client from the list
void removeClient(int client_fd) {
    lock_guard<mutex> lock(clients_mutex);
    
    string username = "Unknown";
    if (client_usernames.find(client_fd) != client_usernames.end()) {
        username = client_usernames[client_fd];
        client_usernames.erase(client_fd);
    }
    
    clients.erase(remove(clients.begin(), clients.end(), client_fd), clients.end());
    close(client_fd);
    cout << "Client '" << username << "' disconnected. Total clients: " << clients.size() << endl;
}

// Function to handle each client in a separate thread
void handleClient(int client_fd) {
    char buffer[MAX_LEN];
    string username;
    
    // First, get the username from client
    string prompt = "Enter your username: ";
    send(client_fd, prompt.c_str(), prompt.length(), 0);
    
    memset(buffer, 0, MAX_LEN);
    int valread = recv(client_fd, buffer, MAX_LEN, 0);
    if (valread <= 0) {
        removeClient(client_fd);
        return;
    }
    
    username = string(buffer);
    // Remove any trailing newline or whitespace
    username.erase(username.find_last_not_of(" \n\r\t") + 1);
    
    if (username.empty()) {
        username = "Anonymous";
    }
    
    // Store username
    {
        lock_guard<mutex> lock(clients_mutex);
        client_usernames[client_fd] = username;
    }
    
    cout << "Client '" << username << "' joined the chatroom" << endl;
    
    string welcome_msg = "*** Welcome to the chatroom, " + username + "! ***\n";
    send(client_fd, welcome_msg.c_str(), welcome_msg.length(), 0);
    
    // Notify other clients about new user
    string join_msg = "*** " + username + " joined the chatroom ***\n";
    broadcastMessage(join_msg, client_fd);
    
    while (true) {
        memset(buffer, 0, MAX_LEN);
        int valread = recv(client_fd, buffer, MAX_LEN, 0);
        
        if (valread <= 0) {
            // Client disconnected
            string leave_msg = "*** " + username + " left the chatroom ***\n";
            broadcastMessage(leave_msg, client_fd);
            removeClient(client_fd);
            break;
        }
        
        // Create formatted message with username
        string message = username + ": " + string(buffer) + "\n";
        cout << "Broadcasting: " << message;
        
        // Broadcast message to all other clients
        broadcastMessage(message, client_fd);
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        cerr << "Socket creation failed\n";
        return 1;
    }

    // Allow socket reuse (helps with quick restart)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        cerr << "Setsockopt failed\n";
        return 1;
    }

    // Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on any network interface
    address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        cerr << "Listen failed\n";
        close(server_fd);
        return 1;
    }

    cout << "=== Chatroom Server Started ===" << endl;
    cout << "Listening on port " << PORT << "..." << endl;
    cout << "Waiting for clients to connect..." << endl;

    // Accept clients in a loop
    while (true) {
        client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_fd < 0) {
            cerr << "Accept failed\n";
            continue;
        }

        // Add client to the list
        {
            lock_guard<mutex> lock(clients_mutex);
            clients.push_back(client_fd);
            cout << "New client connected! Total clients: " << clients.size() << endl;
        }

        // Create a new thread to handle this client
        thread client_thread(handleClient, client_fd);
        client_thread.detach(); // Detach thread so it runs independently
    }

    close(server_fd);
    return 0;
}