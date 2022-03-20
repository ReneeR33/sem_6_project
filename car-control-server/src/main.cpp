#include "Server.h"
#include "Car.h"
#include "CarControlServer.h"

int main()
{
    Car car;
    Server server(5000, 10);

    CarControlServer carControlServer(server, car);

    carControlServer.run();
}