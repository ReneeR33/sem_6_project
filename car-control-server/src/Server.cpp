#include "Server.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <vector>

#define REC_BUF_SIZE 1000

Server::Server(
    int port, int maxConnections
)
    : handler(nullptr)
{
    this->socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if(this->socketfd == -1)
    {
        throw std::runtime_error("Error opening socket: " + std::string(strerror(errno)) + "\n");
    }
    
    memset(&this->addr, 0, sizeof this->addr);

    this->addr.sin_family = AF_INET;
    this->addr.sin_addr.s_addr = INADDR_ANY;

    this->addr.sin_port = htons(port);
    
    if(bind(this->socketfd, (sockaddr*)&this->addr, sizeof this->addr) == -1)
    {
        throw std::runtime_error("Error on binding: " + std::string(strerror(errno)) + "\n");
    }

    listen(socketfd, maxConnections); 
}

Server::~Server()
{
    for(auto clientFD : this->connections)
    {
        disconnect(clientFD);
    }
    close(this->socketfd);
}

void Server::setHandler(IServerHandler *handler)
{
    this->handler = handler;
}

void Server::run()
{
    while (true)
    {
        std::vector<int> removedConnections;

        fd_set readFds;
        FD_ZERO(&readFds);
        FD_SET(this->socketfd, &readFds);

        for (int connection : connections)
        {
            FD_SET(connection, &readFds);
        }

        timeval timout;
        timout.tv_sec = 0;
        timout.tv_usec = 50;
        
        int nrSockets = select(FD_SETSIZE, &readFds, nullptr, nullptr, &timout);

        if(FD_ISSET(this->socketfd, &readFds))
        {
            int connection = acceptConnection();
            if (handler != nullptr)
            {
                handler->onNewConnection(connection);
            }
        }
        else if(nrSockets != -1)
        {
            for(int connection : connections)
            {
                if(FD_ISSET(connection, &readFds))
                {
                    std::string data = receiveData(connection);
                    if (data.empty())
                    {
                        if (handler != nullptr)
                        {
                            handler->onClientDisconnected(connection);
                        }
                        removedConnections.push_back(connection);
                    }
                    else
                    {
                        if (handler != nullptr)
                        {
                            handler->onStringReceived(connection, data);
                        }
                    }
                }
            }
        }

        for (int connection : removedConnections)
        {
            this->connections.erase(std::remove(this->connections.begin(), this->connections.end(), connection));
        }
    }
}

int Server::acceptConnection()
{
    sockaddr_in clientAddress;
    socklen_t clilen = sizeof clientAddress;

    int clientFD = accept(this->socketfd, reinterpret_cast<sockaddr*>(&clientAddress), &clilen);

    if(clientFD == -1)
    {
        throw std::runtime_error("Error on accept: " + std::string(strerror(errno)) + "\n");
    }

    this->connections.push_back(clientFD);

    return clientFD;
}

void Server::disconnect(int clientFD)
{
    this->connections.erase(std::remove(this->connections.begin(), this->connections.end(), clientFD));
    close(clientFD);
}

void Server::sendString(int clientFD, const std::string& str) 
{
    if(write(clientFD, str.c_str(), str.size() + 1) == -1)
    {
        throw std::runtime_error("Error writing to socket: " + std::string(strerror(errno)) + "\n");
    }
}

std::string Server::receiveData(int clientFD) const
{
    std::string result;
    char buf[REC_BUF_SIZE];

    memset(buf, 0, REC_BUF_SIZE);
    ssize_t n;

    n = read(clientFD, buf, REC_BUF_SIZE);

    if(n == -1)
    {
        throw std::runtime_error("Error writing to socket: " + std::string(strerror(errno)) + "\n");
    }

    result = std::string(buf);

    return result;
}