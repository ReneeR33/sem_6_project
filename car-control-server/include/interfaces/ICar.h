#ifndef ICAR_H
#define ICAR_H

class ICar
{
public:
    enum class State
    {
        FOREWARD,
        BACKWARD,
        TURN_LEFT,
        TURN_RIGHT,
        NONE
    };

    virtual ~ICar() = default;

    virtual void setState(State state) = 0;
};

#endif //ICAR_H