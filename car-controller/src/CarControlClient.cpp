#include "CarControlClient.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <stdexcept>

CarControlClient::CarControlClient(ITcpClient& tcpClient)
    : tcpClient(tcpClient)
{
    tcpClient.connect();
}

void CarControlClient::setCarState(CarState state)
{
    switch (state)
    {
    case CarState::FOREWARD:
        std::cout << "sending foreward\n";
        tcpClient.sendString("FOREWARD");
        break;

    case CarState::BACKWARD:
        std::cout << "sending backward\n";
        tcpClient.sendString("BACKWARD");
        break;

    case CarState::TURN_LEFT:
        std::cout << "sending left\n";
        tcpClient.sendString("TURN_LEFT");
        break;

    case CarState::TURN_RIGHT:
        std::cout << "sending right\n";
        tcpClient.sendString("TURN_RIGHT");
        break;

    case CarState::NONE:
        std::cout << "sending none\n";
        tcpClient.sendString("NONE");
        break;
    
    default:
        return;
        break;
    }

    std::string response = tcpClient.receiveString();

    if (response != "ACK")
    {
        throw std::runtime_error("car control client: no response from server");
    }
}