#ifndef NET_H
#define NET_H

#include <string>

class unicast_net {
public:
    unicast_net(int port);
    void send(const std::string &message);
    std::string receive(int &client_id);
};

#endif // NET_H
