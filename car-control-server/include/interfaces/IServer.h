#ifndef ISERVER_H
#define ISERVER_H

#include "IServerHandler.h"
#include <string>

class IServer
{
public:
    virtual ~IServer() = default;

    virtual void setHandler(IServerHandler *handler) = 0;

    virtual void run() = 0;
    virtual void sendString(int clientFD, const std::string& str) = 0;
    virtual void disconnect(int clientFD) = 0;
};

#endif //ISERVER_H