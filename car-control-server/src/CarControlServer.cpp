#include "CarControlServer.h"

#include <iostream>

CarControlServer::CarControlServer(IServer& server, ICar& car)
    : server(server)
    , car(car)
{
    server.setHandler(this);
    std::cout << "starting server\n";
}

void CarControlServer::run()
{
    server.run();
}

void CarControlServer::onNewConnection(int connection)
{
    std::cout << "new connection!\n";
}

void CarControlServer::onClientDisconnected(int connection)
{
    std::cout << "disconnected\n";
}

void CarControlServer::onStringReceived(int connection, const std::string& data)
{
    std::cout << "received data: " << data << '\n';

    if (data == "FOREWARD")
    {
        car.setState(ICar::State::FOREWARD);
    }
    else if (data == "BACKWARD")
    {
        car.setState(ICar::State::BACKWARD);
    }
    else if (data == "TURN_LEFT")
    {
        car.setState(ICar::State::TURN_LEFT);
    }
    else if (data == "TURN_RIGHT")
    {
        car.setState(ICar::State::TURN_RIGHT);
    }
    else if (data == "NONE")
    {
        car.setState(ICar::State::NONE);
    }

    server.sendString(connection, "ACK");
}