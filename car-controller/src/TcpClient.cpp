#include "TcpClient.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>
#include <errno.h>

#include <cstring>
#include <chrono>
#include <stdexcept>

#define READ_BUFF_SIZE 200
#define TIMEOUT 1000

TcpClient::TcpClient(std::string serverAddress, int serverPort)
    : serverAddress(std::move( serverAddress ))
    , serverPort(serverPort)
    , socketFD(-1)
    , connected(false)
{
}

TcpClient::~TcpClient()
{
    disconnect();
}

void TcpClient::connect()
{
    if(this->connected)
    {
        disconnect();
    }

    this->socketFD = socket(AF_INET, SOCK_STREAM, 0);

    if(this->socketFD == -1)
    {
        throw std::runtime_error("Error operning socket: " + std::string(strerror(errno) + '\n'));
    }

    timeval tv;
    tv.tv_sec = TIMEOUT / 1000;
    tv.tv_usec = TIMEOUT % 1000;

    setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    hostent* serverData = gethostbyname(serverAddress.c_str());

    if(serverData == nullptr)
    {
        throw std::runtime_error("Failed to connect to server: " + std::string(strerror(errno) + '\n'));
    }

    bzero((char *) &this->addr, sizeof(this->addr));
    this->addr.sin_family = AF_INET;
    bcopy((char *)serverData->h_addr, (char *)&this->addr.sin_addr.s_addr, serverData->h_length);

    this->addr.sin_port = htons(serverPort);

    if(::connect(this->socketFD, (sockaddr*)&this->addr, sizeof this->addr) < 0)
    {
        throw std::runtime_error("Failed to connect to server: " + std::string(strerror(errno) + '\n'));
    }

    this->connected = true;
}

void TcpClient::disconnect()
{
    if(!this->connected)
    {
        return;
    }
    
    this->connected = false;

    close(this->socketFD);
    this->socketFD = -1;
}

void TcpClient::sendString(const std::string& data)
{
    if(!this->connected)
    {
        return;
    }

    if(write(this->socketFD, data.c_str(), data.size() + 1) == -1)
    {
        throw std::runtime_error("Error writing to socket: " + std::string(strerror(errno) + '\n'));
    }
}

std::string TcpClient::receiveString()
{
    char buff[READ_BUFF_SIZE];
    std::string result;

    ssize_t n = read(this->socketFD, buff, READ_BUFF_SIZE);

    if (n == -1)
    {
        throw std::runtime_error("Error reading from socket: " + std::string(strerror(errno) + '\n'));
    }

    result = std::string(buff);

    return result;
}

int TcpClient::bytesAvailable() const
{
    int bytesAvailable = 0;

    ioctl(this->socketFD, FIONREAD, &bytesAvailable);

    return bytesAvailable;
}