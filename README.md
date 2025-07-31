## Introduction

This project is a simple chatroom application that you can use in your terminal/command prompt. It enables real-time messaging between multiple users by leveraging WebSockets—a technology that allows interactive communication between a client (your terminal) and a server.

---

## Why Use WebSockets?

Most traditional network communication over the web uses HTTP, which is great for loading web pages, but not for real-time apps. HTTP works like sending a letter: you send a request, the server replies, and that's it.

WebSocket, on the other hand, is like a phone call: once the connection is established, both sides can send messages to each other at any time, instantly. This is ideal for chat apps where you want to see messages from others as soon as they are sent.

**Example Scenario:**
- Tom and Emi both open the chatroom in their terminals.
- Tom types "Hi Emi!" and sends it.
- Instantly, Emi sees "Tom: Hi Emi!" in his terminal, without needing to refresh or press any extra keys.

<img width="1593" height="876" alt="Screenshot From 2025-07-31 17-51-17" src="https://github.com/user-attachments/assets/648a04fc-2ff2-4eff-aa50-a43bca4afa6d" />


---

## Features

- Real-time messaging between multiple users
- Text-based (terminal) user interface
- Written in C++ for performance and learning
- Uses WebSockets for efficient, two-way communication

---

## How It Works (Simplified)

1. A server runs and listens for incoming WebSocket connections.
2. Clients (users) connect to the server from their terminals.
3. When a user sends a message, the server broadcasts it to all connected users.
4. All users see new messages appear in real-time in their terminals.


---

## Getting Started

### 1. Clone the repository

```sh
git clone https://github.com/parth721/chatroom.git
cd chatroom
```
`

### 3. Build the project

```sh
make
```

### 4. Run the server

```sh
./server
```

### 5. Connect with clients

Open another terminal for each user and run:

```sh
./client
```

---

## Example Usage

**Server Terminal:**
```
[Server] Listening on ws://localhost:12345
```

**Client 1 Terminal:**
```
Enter username: Tom
[Tom] Hi Emi!
[Emi] Hello Tom!
```

**Client 2 Terminal:**
```
Enter username: Emi
[Tom] Hi Emi!
[Emi] Hello Tom!
```

---

## Underlying Concepts Explained

### What are WebSockets?

- WebSockets create a persistent connection between client and server, allowing data to flow in both directions, instantly.
- Unlike HTTP, there's no need to repeatedly ask the server for new data ("polling").
- Great for chat apps, games, live notifications, collaborative editing, etc.

### What is a CLI (Command-Line Interface)?

- A CLI is a text-based way to interact with programs. You type commands, see output in your console/terminal, and interact using your keyboard.

### Why C++?

- C++ is fast and efficient, making it suitable for real-time applications.
- This project is also a learning opportunity to work with networking and concurrency in C++.

---

## Next Steps & Contributions

- Add user authentication
- Support private messaging
- Improve user interface/UX
- Add message history

If you'd like to contribute, feel free to fork the repo and create a pull request!

---

## License

MIT License

---

## Contact

Maintainer: [@parth721](https://github.com/parth721)

If you have questions or suggestions, open an issue or reach out on GitHub.
