#include "net.h"
#include <windows.h>
#include <ctime>
#include <map>

MovableObject *my_vehicle;
unicast_net *uni_reciv;
unicast_net *uni_send;
HANDLE threadReciv;
DWORD dwThreadId;

void InteractionInitialisation() {
    my_vehicle = new MovableObject();
    time_of_cycle = clock();

    uni_reciv = new unicast_net(1001); // Receiving messages
    uni_send = new unicast_net(1002);  // Sending messages

    threadReciv = CreateThread(
        NULL,
        0,
        ReceiveThreadFun,
        (void *)uni_reciv,
        NULL,
        &dwThreadId);
}

void VirtualWorldCycle() {
    // Update the state of the virtual world
    // Send the current state of the user's vehicle to the server
    uni_send->send(my_vehicle->getState());
}

DWORD WINAPI ReceiveThreadFun(void *ptr) {
    unicast_net *uni_reciv = (unicast_net *)ptr;
    while (true) {
        // Receive messages from the server
        uni_reciv->receive();
    }
    return 0;
}

int main() {
    // Initialize graphics and interaction
    InteractionInitialisation();
    // Main loop
    while (true) {
        VirtualWorldCycle();
    }
    return 0;
}
