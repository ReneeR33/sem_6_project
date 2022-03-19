#ifndef WHEELS_H
#define WHEELS_H

typedef enum
{
    WHEELS_FOREWARD = 0,
    WHEELS_BACKWARD = 1,
    WHEELS_NONE     = 2
} WheelState;

typedef struct
{
    int gpio_foreward;
    int gpio_backward;
    WheelState state;
} Wheels;

int wheels_init(Wheels *wheels, int gpio_foreward, int gpio_backward);
void wheels_free(Wheels *wheels);
void wheels_set_state(Wheels *wheels, WheelState state);

#endif //WHEELS_H