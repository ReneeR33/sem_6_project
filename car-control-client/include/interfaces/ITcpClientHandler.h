#ifndef ITCP_CLIENT_HANDLER_HPP
#define ITCP_CLIENT_HANDLER_HPP

#include <string>

class ITcpClientHandler
{
public:
    virtual ~ITcpClientHandler() = default;

    virtual void onTcpStringReceived(const std::string& data) = 0;
    virtual void onTcpConnectionLost() = 0;
};


#endif //ITCP_CLIENT_HANDLER_HPP