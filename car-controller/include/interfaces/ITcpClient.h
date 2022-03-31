#ifndef ITCP_CLIENT_HPP
#define ITCP_CLIENT_HPP

#include <vector>
#include <string>


class ITcpClient
{
public:
    virtual ~ITcpClient() = default;
    
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void sendString(const std::string& data) = 0;
    virtual std::string receiveString() = 0;
};


#endif //ITCP_CLIENT_HPP