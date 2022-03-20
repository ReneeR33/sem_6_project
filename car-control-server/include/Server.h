#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <exception>
#include <string>
#include <vector>
#include <functional>
#include "interfaces/IServer.h"
#include "interfaces/IServerHandler.h"

class Server : public IServer
{
public:
    Server(
        int port, int maxConnections
    );
    virtual ~Server();

    void setHandler(IServerHandler *handler) override;

    void run() override;
    void sendString(int clientFD, const std::string& str) override;
    void disconnect(int clientFD) override;

private:
    int socketfd;
    sockaddr_in addr;
    IServerHandler *handler;

    std::vector<int> connections;

    int acceptConnection();
    std::string receiveData(int clientFD) const;
};

#endif //SERVER_H