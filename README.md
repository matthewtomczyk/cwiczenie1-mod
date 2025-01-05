### **Cwiczenie 3:**

### **Variables:**
Add variables:
```cpp
bool collaboration_in_progress = false;
bool negotiation_active = false;
float negotiation_ratio = 50.0f;  // Default 50-50 split
int negotiation_partner_ID = -1;
float fuel_price = 0.0f;
```

---

### **Method 1: Start Collaboration Request**
This method initiates collaboration by sending a request to another participant.
```cpp
void SendCollaborationRequest(int receiver_ID) {
    Frame frame;
    frame.frame_type = TRANSFER;
    frame.iID_receiver = receiver_ID;
    frame.transfer_type = MONEY;
    frame.transfer_value = 0;  // Placeholder for initial offer
    frame.iID = my_vehicle->iID;

    multi_send->send((char*)&frame, sizeof(Frame));
    sprintf(par_view.inscription2, "Wysłano zaproszenie do współpracy (ID %d)", receiver_ID);
}
```

---

### **Method 2: Respond to Collaboration Request**
Handles incoming collaboration requests and allows the user to accept or reject them.
```cpp
void RespondToCollaboration(bool accept, int partner_ID) {
    if (accept) {
        collaboration_in_progress = true;
        negotiation_partner_ID = partner_ID;
        sprintf(par_view.inscription2, "Współpraca nawiązana z ID %d", partner_ID);
    } else {
        sprintf(par_view.inscription2, "Odrzucono współpracę z ID %d", partner_ID);
    }
}
```

---

### **Method 3: Negotiation for Profit/Fuel Split**
Negotiation to determine the percentage split of resources (e.g., fuel or money).
```cpp
void StartNegotiation(int partner_ID) {
    negotiation_active = true;
    negotiation_partner_ID = partner_ID;
    negotiation_ratio = 50.0f;  // Start at 50-50 split
    sprintf(par_view.inscription2, "Rozpoczęto negocjacje z ID %d", partner_ID);
}

void AdjustNegotiation(float new_ratio) {
    negotiation_ratio = new_ratio;
    sprintf(par_view.inscription2, "Proponowany podział: %.1f%% dla ID %d", negotiation_ratio, negotiation_partner_ID);
}

void ConfirmNegotiation() {
    negotiation_active = false;
    TransferSending(negotiation_partner_ID, MONEY, my_vehicle->state.money * (negotiation_ratio / 100));
    sprintf(par_view.inscription2, "Potwierdzono podział: %.1f%%", negotiation_ratio);
}
```

---

### **Method 4: Auction for Fuel (Basic Implementation)**
```cpp
void StartFuelAuction(float starting_price) {
    fuel_price = starting_price;
    sprintf(par_view.inscription2, "Rozpoczęto licytację paliwa. Cena: %.2f", fuel_price);
}

void PlaceBid(float bid) {
    if (bid > fuel_price) {
        fuel_price = bid;
        sprintf(par_view.inscription2, "Nowa oferta: %.2f", fuel_price);
    } else {
        sprintf(par_view.inscription2, "Oferta za niska. Aktualna cena: %.2f", fuel_price);
    }
}

void FinalizeAuction(int winner_ID) {
    TransferSending(winner_ID, FUEL, 10);  // Transfer 10 units of fuel
    sprintf(par_view.inscription2, "Zakończono licytację. ID %d wygrywa za %.2f", winner_ID, fuel_price);
}
```

---

### **Method 5: Message Box Prompt (Optional)**
```cpp
void ShowNegotiationPrompt(int sender_ID) {
    int result = MessageBox(main_window, "Czy chcesz podjąć współpracę?", "Zaproszenie", MB_YESNO);
    RespondToCollaboration(result == IDYES, sender_ID);
}
```

---

### **Implementation Notes:**
- **Triggers:**  
  - Collaboration starts when a request is received.  
  - Negotiations adjust values dynamically until confirmed by both parties.  
  - Auctions run with time limits or until a user accepts the highest bid.  

- **Synchronization:**  
  Use the existing **critical section (`EnterCriticalSection`)** to ensure thread safety during network communications and negotiation processes.  

Would you like to integrate these directly into the existing message-handling loop or as callable functions from keyboard inputs?

### Client Application

The client application initializes a vehicle object and uses unicast networking to send and receive messages to and from the server.

### Key Components

- **Initialization**: Initializes the vehicle object and sets up unicast networking for communication.
- **Virtual World Cycle**: Updates the state of the virtual world and sends the current state of the user's vehicle to the server.
- **Message Handling**: Receives messages from the server in a separate thread.

## Compilation Instructions

### Visual C++

Add the following libraries to the project options:
- WSOCK32.LIB
- MPR.LIB

### DEV-C++

Add the following library to the project options:
- -libws2_32

## Server Application

The server application registers clients, receives messages from them, and sends messages to all registered clients. It also handles inactive clients.

### Key Components

- **Client Registration**: Registers and removes clients.
- **Message Reception**: Receives messages from clients and identifies inactive clients.
- **Message Distribution**: Sends received messages to all registered clients.

## Networking Module

The networking module handles unicast networking for sending and receiving messages.

### Key Components

- **Unicast Networking**: Manages sending and receiving messages over unicast.

## Running the Application

1. Start the server application on one computer.
2. Start the client application on two other computers.
3. Ensure the server and clients use the correct IP addresses and ports for communication.

## Notes

- The server and clients use separate ports for sending and receiving messages.
- The server registers clients, receives messages, and sends messages to all registered clients.
- The server handles inactive clients by removing them from the list of registered clients.

## Example Usage

- **Initialize Graphics and Interaction**: The client application initializes graphics and sets up interaction.
- **Main Loop**: The client application runs a main loop that updates the virtual world and communicates with the server.
- **Server Operations**: The server application runs threads to handle message reception and distribution.

This setup allows the client and server applications to communicate in a client-server architecture, with the server managing client registration and message distribution.


## Temat 2: Komunikacja w zależności od architektury sieciowej
W ramach ćwiczenia należy wykonać podany przez prowadzącego wariant
zadania z użyciem zadanej architektury (klient-serwer lub multicast).
### Zadania:
1. Funkcja pozwalająca na zachowanie stanu obiektu oraz identyfikację użytkowania, tak aby stan obiektu własnego po ponownym uruchomieniu programu na tym samym lub innym komputerze był taki sam (należy więc tę informację przechowywać w pamięci aplikacji, a nie np. na dysku lokalnym). Identyfikacja użytkownika - uwierzytelnienie (system logowania) może być uproszczone do podawania ostatniej cyfry numeru ID.
2. Wybór miejsca dla nowego pojazdu, tak aby zjawiał się w miejscu możliwie
najmniej widocznym przez innych użytkowników (zakładając, że mają
ustawiony widok z kokpitu) lub jak najdalszym od innego, najbliżej położonego
pojazdu. Odległość można obliczyć licząc długość wektora pomiędzy środkami
pojazdów. W przypadku terenu płaskiego należy pominąć składową y. Można
losować położenie pojazdu np. 10 razy, a następnie wybrać położenie
najbardziej odległe od innych użytkowników.
3. Zamiana pojazdów pomiędzy dwoma użytkownikami na ich życzenie
(potrzebna jest do tego celu ramka specjalnego typu). Wybór pojazdu innego
użytkownika może odbywać się poprzez podanie jego numeru ID, znalezienie
się w małej odległości lub w inny sposób zapewniający jednoznaczność
wyboru. Niedopuszczalna jest możliwość wystąpienia sytuacji, gdy jeden z
użytkowników się zamienił, a drugi nie.
4. Wykrywanie kolizji (np. poprzez porównywanie położeń pojazdów - vPos) i
poinformowanie każdego z uczestników . Pojazdy biorące udział w kolizji
powinny się zatrzymać. Kontynuacja ruchu powinna być możliwa tylko w
kierunku, w którym kolizja nie jest pogłębiana. Uwaga, należy sprawdzić, czy
funkcja zadziała przy dużych prędkościach ruchu obiektów. Kształty obiektów
można uprościć do sfer opisujących te obiekty, np. korzystając ze wzoru
(vPos1-vPos2).length() < radius1+radius2. Moment kolizji
powinien zostać uzgodniony przez obie aplikacje lub z wykorzystaniem trzeciej
aplikacji. Niedopuszczalna jest sytuacja, gdy jeden z użytkowników uznał
kolizję, a drugi nie.
5. System głosowania: przyjęcie każdego nowego uczestnika grupy roboczej
wymaga uzyskania większości głosów pozostałych uczestników. Aplikacja
nowego uczestnika nie może uczestniczyć w liczeniu głosów ani w
podejmowaniu decyzji o przyjęciu.

## Implementation
1. Save and Restore Object State
Add functions to save and restore the state of the object in memory.

```cpp
// In objects.h
class MovableObject {
public:
    // Existing members...
    void SaveState();
    void RestoreState();
    static int Authenticate(int id);
};

// In objects.cpp
static ObjectState saved_state;
static int saved_id;

void MovableObject::SaveState() {
    saved_state = state;
    saved_id = iID;
}

void MovableObject::RestoreState() {
    state = saved_state;
    iID = saved_id;
}

int MovableObject::Authenticate(int id) {
    return id % 10;
}
```

```cpp
// In objects.h
class MovableObject {
public:
    // Existing members...
    void SaveState();
    void RestoreState();
    static int Authenticate(int id);
};

// In objects.cpp
static ObjectState saved_state;
static int saved_id;

void MovableObject::SaveState() {
    saved_state = state;
    saved_id = iID;
}

void MovableObject::RestoreState() {
    state = saved_state;
    iID = saved_id;
}

int MovableObject::Authenticate(int id) {
    return id % 10;
}
```

2. Choose Position for New Vehicle
Implement a function to choose a position for a new vehicle that is least visible or farthest from other vehicles.

```cpp
// In objects.cpp
Vector3 ChooseNewPosition() {
    Vector3 best_position;
    float max_distance = 0;

    for (int i = 0; i < 10; ++i) {
        Vector3 new_position(rand() % 100, 0, rand() % 100);
        float min_distance = FLT_MAX;

        for (auto& pair : movable_objects) {
            MovableObject* obj = pair.second;
            float distance = (new_position - obj->state.vPos).length();
            if (distance < min_distance) {
                min_distance = distance;
            }
        }

        if (min_distance > max_distance) {
            max_distance = min_distance;
            best_position = new_position;
        }
    }

    return best_position;
}
```
3. Swap Vehicles Between Users
Implement a function to handle vehicle swapping between users.

```cpp
// In main.cpp
void SwapVehicles(int id1, int id2) {
    if (movable_objects.find(id1) != movable_objects.end() && movable_objects.find(id2) != movable_objects.end()) {
        MovableObject* obj1 = movable_objects[id1];
        MovableObject* obj2 = movable_objects[id2];

        std::swap(obj1->state, obj2->state);
        std::swap(obj1->iID, obj2->iID);
    }
}
```
4. Collision Detection and Handling
Implement a function to detect collisions between vehicles and handle them.

```cpp
// In objects.cpp
void DetectAndHandleCollisions() {
    for (auto& pair1 : movable_objects) {
        MovableObject* obj1 = pair1.second;
        for (auto& pair2 : movable_objects) {
            if (pair1.first == pair2.first) continue;

            MovableObject* obj2 = pair2.second;
            if ((obj1->state.vPos - obj2->state.vPos).length() < obj1->radius + obj2->radius) {
                obj1->state.vV = Vector3(0, 0, 0);
                obj2->state.vV = Vector3(0, 0, 0);
            }
        }
    }
}
```

5. Voting System for New Participants
Implement a voting system where existing participants vote to accept a new participant.

```cpp
// In main.cpp
bool VoteForNewParticipant(int new_id) {
    int votes = 0;
    int total_participants = movable_objects.size();

    for (auto& pair : movable_objects) {
        if (pair.first != new_id) {
            // Simulate a vote (for simplicity, assume everyone votes yes)
            votes++;
        }
    }

    return votes > total_participants / 2;
}
```

## Integration
Integrate these functions into the main application flow.

```cpp
// In main.cpp
int main() {
    // Initialization code...

    // Authenticate user
    int user_id = MovableObject::Authenticate(input_id);
    if (user_id != expected_id) {
        printf("Authentication failed\n");
        return 1;
    }

    // Restore state if available
    my_vehicle->RestoreState();

    // Main loop
    while (true) {
        // Handle new vehicle position
        my_vehicle->state.vPos = ChooseNewPosition();

        // Handle vehicle swapping
        if (swap_request) {
            SwapVehicles(my_vehicle->iID, target_id);
        }

        // Detect and handle collisions
        DetectAndHandleCollisions();

        // Handle voting for new participants
        if (new_participant_request) {
            if (VoteForNewParticipant(new_participant_id)) {
                printf("New participant accepted\n");
            } else {
                printf("New participant rejected\n");
            }
        }

        // Save state periodically
        my_vehicle->SaveState();

        // Other application logic...
    }

    return 0;
}
```
