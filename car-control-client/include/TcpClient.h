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
    
    std::string getServerAddress() override;
    int getServerPort() override;
    bool getConnected() override;

    void addHandler(ITcpClientHandler* handler) override;
    void connect() override;
    void disconnect() override;
    void sendString(const std::string& data) override;

private:
    std::string serverAddress;
    int serverPort;

    int socketFD;
    sockaddr_in addr;

    std::mutex mtx;
    std::condition_variable connected_cv;
    std::atomic<bool> connected;
    std::atomic<bool> quit;

    std::vector<ITcpClientHandler*> handlers;
    std::unique_ptr<std::thread> thread;

    int bytesAvailable() const;
    std::string getData();

    void watch();
};


#endif //TCP_CLIENT_H