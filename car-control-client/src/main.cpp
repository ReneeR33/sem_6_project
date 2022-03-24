#include "TcpClient.h"
#include "CarControlClient.h"
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
    TcpClient tcpClient("127.0.0.1", 5000);
    CarControlClient carControlClient(tcpClient);

    for (int i = 0; i < 2; i++)
    {
        carControlClient.setCarState(CarState::FOREWARD);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        carControlClient.setCarState(CarState::BACKWARD);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    carControlClient.setCarState(CarState::NONE);
}