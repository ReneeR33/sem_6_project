#ifndef ITCP_CLIENT_HPP
#define ITCP_CLIENT_HPP

#include "ITcpClientHandler.h"
#include <vector>
#include <string>


class ITcpClient
{
public:
    virtual ~ITcpClient() = default;

    virtual std::string getServerAddress() = 0;
    virtual int getServerPort() = 0;
    virtual bool getConnected() = 0;
    
    virtual void addHandler(ITcpClientHandler* handler) = 0;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void sendString(const std::string& data) = 0;
};


#endif //ITCP_CLIENT_HPP