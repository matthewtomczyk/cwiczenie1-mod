#include "net.h"
#include <map>
#include <vector>
#include <thread>
#include <iostream>

std::map<int, unicast_net*> clients;
std::vector<int> inactive_clients;

void registerClient(int client_id, unicast_net *client_net) {
    clients[client_id] = client_net;
}

void removeClient(int client_id) {
    clients.erase(client_id);
}

void receiveMessages() {
    unicast_net uni_reciv(1001); // Receiving messages from clients
    while (true) {
        int client_id;
        std::string message = uni_reciv.receive(client_id);
        if (message.empty()) {
            inactive_clients.push_back(client_id);
        } else {
            for (auto &client : clients) {
                if (client.first != client_id) {
                    client.second->send(message);
                }
            }
        }
    }
}

void sendMessages() {
    unicast_net uni_send(1002); // Sending messages to clients
    while (true) {
        for (int client_id : inactive_clients) {
            removeClient(client_id);
        }
        inactive_clients.clear();
    }
}

int main() {
    std::thread recv_thread(receiveMessages);
    std::thread send_thread(sendMessages);

    recv_thread.join();
    send_thread.join();

    return 0;
}
