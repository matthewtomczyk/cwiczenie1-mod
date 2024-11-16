Virtual Collaborative Teams - Client-Server Architecture
This project demonstrates a simple client-server architecture for a virtual collaborative teams application. The client application communicates with the server to share state information about various objects in the simulation.

Client Application
The client application initializes a vehicle object and uses unicast networking to send and receive messages to and from the server.

Key Components
Initialization: Initializes the vehicle object and sets up unicast networking for communication.
Virtual World Cycle: Updates the state of the virtual world and sends the current state of the user's vehicle to the server.
Message Handling: Receives messages from the server in a separate thread.
Compilation Instructions
Visual C++: Add the following libraries to the project options:
WSOCK32.LIB
MPR.LIB
DEV-C++: Add the following library to the project options:
-libws2_32
Server Application
The server application registers clients, receives messages from them, and sends messages to all registered clients. It also handles inactive clients.

Key Components
Client Registration: Registers and removes clients.
Message Reception: Receives messages from clients and identifies inactive clients.
Message Distribution: Sends received messages to all registered clients.
Networking Module
The networking module handles unicast networking for sending and receiving messages.

Key Components
Unicast Networking: Manages sending and receiving messages over unicast.
Running the Application
Start the server application on one computer.
Start the client application on two other computers.
Ensure the server and clients use the correct IP addresses and ports for communication.
Notes
The server and clients use separate ports for sending and receiving messages.
The server registers clients, receives messages, and sends messages to all registered clients.
The server handles inactive clients by removing them from the list of registered clients.
Example Usage
Initialize Graphics and Interaction: The client application initializes graphics and sets up interaction.
Main Loop: The client application runs a main loop that updates the virtual world and communicates with the server.
Server Operations: The server application runs threads to handle message reception and distribution.
This setup allows the client and server applications to communicate in a client-server architecture, with the server managing client registration and message distribution.
