#ifndef ISERVER_HANDLER_H
#define ISERVER_HANDLER_H

#include <string>

class IServerHandler
{
public:
    virtual ~IServerHandler() = default;

    virtual void onNewConnection(int connection) = 0;
    virtual void onStringReceived(int connection, const std::string& data) = 0;
};

#endif //ISERVER_HANDLER_H