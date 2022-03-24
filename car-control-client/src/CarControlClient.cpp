#include "CarControlClient.h"

#include <thread>
#include <chrono>
#include <iostream>

CarControlClient::CarControlClient(ITcpClient& tcpClient)
    : tcpClient(tcpClient)
{
    tcpClient.addHandler(this);
    tcpClient.connect();
}

void CarControlClient::setCarState(CarState state)
{
    switch (state)
    {
    case CarState::FOREWARD:
        tcpClient.sendString("FOREWARD");
        break;

    case CarState::BACKWARD:
        tcpClient.sendString("BACKWARD");
        break;

    case CarState::TURN_LEFT:
        tcpClient.sendString("TURN_LEFT");
        break;

    case CarState::TURN_RIGHT:
        tcpClient.sendString("TURN_RIGHT");
        break;

    case CarState::NONE:
        tcpClient.sendString("NONE");
        break;
    
    default:
        break;
    }
}

void CarControlClient::onTcpStringReceived(const std::string& data)
{
}

void CarControlClient::onTcpConnectionLost()
{
}