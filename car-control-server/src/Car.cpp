#include "Car.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string>
#include <stdexcept>
#include <string.h>
#include "car-chassis-driver.h"

Car::Car()
{
    int fd = open("/dev/car-chassis", 0);

    if (fd == -1)
    {
        throw std::runtime_error("error opening car-chassis: " + std::string(strerror(errno)) + "\n");
    }
}

Car::~Car()
{
    close(fd);
}

void Car::setState(ICar::State state)
{
    switch (state)
    {
    case ICar::State::FOREWARD:
        ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_FOREWARD);
        ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_FOREWARD);
        break;

    case ICar::State::BACKWARD:
        ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_BACKWARD);
        ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_BACKWARD);
        break;

    case ICar::State::TURN_LEFT:
        ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_FOREWARD);
        ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_BACKWARD);
        break;

    case ICar::State::TURN_RIGHT:
        ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_BACKWARD);
        ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_FOREWARD);
        break;

    case ICar::State::NONE:
        ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_NONE);
        ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_NONE);
        break;
    
    default:
        break;
    }
}