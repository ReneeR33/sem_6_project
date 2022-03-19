#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <string>
#include <string.h>
#include "car-chassis-driver.h"

int main()
{
    int fd = open("/dev/car-chassis", 0);

    if (fd == -1)
    {
        std::cout << "error: " << std::string(strerror(errno)) << "\n";
        return -1;
    }

    ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_FOREWARD);
    ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_FOREWARD);

    sleep(3);

    ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_BACKWARD);
    ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_BACKWARD);

    sleep(3);

    ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_BACKWARD);
    ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_FOREWARD);

    sleep(3);

    ioctl(fd, CAR_CHASSIS_IOC_SL_WHEELS_STATE, WHEELS_NONE);
    ioctl(fd, CAR_CHASSIS_IOC_SR_WHEELS_STATE, WHEELS_NONE);

    close(fd);
}