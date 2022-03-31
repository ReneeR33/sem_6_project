#ifndef CAR_CONTROL_CLIENT
#define CAR_CONTROL_CLIENT

#include "interfaces/ITcpClient.h"
#include "interfaces/ICarControlClient.h"

#include <mutex>
#include <condition_variable>

class CarControlClient : ICarControlClient
{
public:
    CarControlClient(ITcpClient& tcpClient);

    void setCarState(CarState state) override;

private:
    ITcpClient& tcpClient;
};

#endif //CAR_CONTROL_CLIENT