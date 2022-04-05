#include "TcpClient.h"
#include "CarControlClient.h"
#include "EventQueue.h"
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
    TcpClient tcpClient("192.168.178.80", 5000);
    CarControlClient carControlClient(tcpClient);
    EventQueue eventQueue;

    while (true)
    {
        std::cout << "waiting for event...\n";
        Event event = eventQueue.getEvent();
        std::cout << "got event!, pressed: " << event.pressed << ", button: " << event.button << "\n";

        if (event.pressed)
        {
            switch(event.button)
            {
            case 0:
                carControlClient.setCarState(CarState::TURN_LEFT);
                break;
            
            case 1:
                carControlClient.setCarState(CarState::BACKWARD);
                break;

            case 2:
                carControlClient.setCarState(CarState::FOREWARD);
                break;

            case 3:
                carControlClient.setCarState(CarState::TURN_RIGHT);
                break;

            default:
                break;
            }
        }
        else if (!event.pressed)
        {
            carControlClient.setCarState(CarState::NONE);
        }
    }
}