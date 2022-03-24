#ifndef ICAR_CONTROL_CLIENT_H
#define ICAR_CONTROL_CLIENT_H

#include "../CarState.h"

class ICarControlClient
{
public:
    virtual ~ICarControlClient() = default;

    virtual void setCarState(CarState state) = 0;
};

#endif //ICAR_CONTROL_CLIENT_H