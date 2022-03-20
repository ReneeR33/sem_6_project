#ifndef CAR_CONTROL_SERVER_H
#define CAR_CONTROL_SERVER_H

#include "interfaces/ICar.h"
#include "interfaces/IServer.h"
#include "interfaces/IServerHandler.h"

class CarControlServer : public IServerHandler
{
public:
    CarControlServer(IServer& server, ICar& car);

    void run();

    void onNewConnection(int connection) override;
    void onStringReceived(int connection, const std::string& data) override;

private:
    IServer& server;
    ICar& car;
};

#endif //CAR_CONTROL_SERVER_H