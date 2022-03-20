#include "CarControlServer.h"

#include <iostream>

CarControlServer::CarControlServer(IServer& server, ICar& car)
    : server(server)
    , car(car)
{
    server.setHandler(this);
}

void CarControlServer::run()
{
    server.run();
}

void CarControlServer::onNewConnection(int connection)
{
    std::cout << "new connection!\n";
}

void CarControlServer::onStringReceived(int connection, const std::string& data)
{
    std::cout << "received data!\n";
}