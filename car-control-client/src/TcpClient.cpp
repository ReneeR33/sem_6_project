#include "TcpClient.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>

#include <cstring>
#include <chrono>
#include <stdexcept>


TcpClient::TcpClient(std::string serverAddress, int serverPort)
    : serverAddress(std::move( serverAddress ))
    , serverPort(serverPort)
    , socketFD(-1)
    , connected(false)
    , quit(false)
{
}

TcpClient::~TcpClient()
{
    disconnect();

    this->quit = true;
    connected_cv.notify_all();
    
    if (this->thread && this->thread->joinable())
    {
        this->thread->join();
    }
}

std::string TcpClient::getServerAddress()
{
    return serverAddress;
}

int TcpClient::getServerPort()
{
    return serverPort;
}

bool TcpClient::getConnected()
{
    return connected;
}

void TcpClient::addHandler(ITcpClientHandler* handler)
{
    this->handlers.push_back( handler );
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
    connected_cv.notify_all();
    
    if (!this->thread)
    {
        this->thread = std::make_unique<std::thread>(&TcpClient::watch, this);
    }
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

std::string TcpClient::getData()
{
    std::string result;

    int bytesAvailable = this->bytesAvailable();

    if (bytesAvailable <= 0)
    {
        return result;
    }

    char* buffer = new char[bytesAvailable];

    ssize_t n = read(this->socketFD, buffer, bytesAvailable);

    if (n == -1)
    {
        throw std::runtime_error("Error reading from socket: " + std::string(strerror(errno) + '\n'));
    }

    result = std::string(buffer);

    delete[] buffer;

    return result;
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

int TcpClient::bytesAvailable() const
{
    int bytesAvailable = 0;

    ioctl(this->socketFD, FIONREAD, &bytesAvailable);

    return bytesAvailable;
}

void TcpClient::watch()
{
    while (!quit)
    {
        while(connected && !quit)
        {
            fd_set read_fs;

            FD_ZERO(&read_fs);
            FD_SET(this->socketFD, &read_fs);

            timeval timout;
            timout.tv_sec = 0;
            timout.tv_usec = 500;

            int nrSockets = select(FD_SETSIZE, &read_fs, nullptr, nullptr, &timout);

            if (nrSockets > 0)
            {
                std::string received = getData();

                if (received.empty())
                {
                    for (auto& handler : this->handlers)
                    {
                        connected = false;
                        handler->onTcpConnectionLost();
                    }
                }
                else
                {
                    for (auto& handler : this->handlers)
                    {
                        handler->onTcpStringReceived(received);
                    }
                }
            }
        }

        if (quit)
        {
            return;
        }

        {
            std::unique_lock<std::mutex> lck(mtx);
            connected_cv.wait(lck);
        }
    }
}