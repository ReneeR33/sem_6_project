#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "interfaces/ITcpClient.h"
#include <netinet/in.h>
#include <exception>
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <mutex>              
#include <condition_variable>


class TcpClient : public ITcpClient
{
public:
    TcpClient(std::string serverAddress, int serverPort);
    ~TcpClient();
    
    void connect() override;
    void disconnect() override;
    void sendString(const std::string& data) override;
    std::string receiveString() override;

private:
    std::string serverAddress;
    int serverPort;

    int socketFD;
    sockaddr_in addr;

    bool connected;

    int bytesAvailable() const;
};


#endif //TCP_CLIENT_H