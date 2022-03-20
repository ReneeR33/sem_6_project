#ifndef CAR_H
#define CAR_H

#include "interfaces/ICar.h"

class Car : public ICar
{
public:
    Car();
    ~Car();

    void setState(ICar::State state) override;

private:
    int fd;
};

#endif //CAR_H