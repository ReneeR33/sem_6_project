#ifndef CAR_CONTROL_CLIENT
#define CAR_CONTROL_CLIENT

#include "interfaces/ITcpClient.h"
#include "interfaces/ITcpClientHandler.h"
#include "interfaces/ICarControlClient.h"

#include <mutex>
#include <condition_variable>

class CarControlClient : public ITcpClientHandler, ICarControlClient
{
public:
    CarControlClient(ITcpClient& tcpClient);

    void setCarState(CarState state) override;

    void onTcpStringReceived(const std::string& data) override;
    void onTcpConnectionLost() override;

private:
    ITcpClient& tcpClient;
};

#endif //CAR_CONTROL_CLIENT