#include "wheels.h"

#include <linux/gpio.h>
#include "gpio.h"

int wheels_init(Wheels *wheels, int gpio_foreward, int gpio_backward)
{
    int error;

    if ((error = gpio_init(gpio_foreward)) != 0)
    {
        return error;
    }
    gpio_direction_output(gpio_foreward, 0);

    if ((error = gpio_init(gpio_backward)) != 0)
    {
        goto err_free_gpio_foreward;
    }
    gpio_direction_output(gpio_backward, 0);

    wheels->gpio_foreward = gpio_foreward;
    wheels->gpio_backward = gpio_backward;
    wheels->state = WHEELS_NONE;

    return 0;

err_free_gpio_foreward:
    gpio_free(gpio_foreward);

    return error;
}

void wheels_free(Wheels *wheels)
{
    gpio_set_value(wheels->gpio_foreward, 0);
    gpio_set_value(wheels->gpio_backward, 0);

    gpio_free(wheels->gpio_foreward);
    gpio_free(wheels->gpio_backward);
}

void wheels_set_state(Wheels *wheels, WheelState state)
{
    switch(state)
    {
    case WHEELS_FOREWARD:
        gpio_set_value(wheels->gpio_foreward, 1);
        gpio_set_value(wheels->gpio_backward, 0);

        break;
    
    case WHEELS_BACKWARD:
        gpio_set_value(wheels->gpio_foreward, 0);
        gpio_set_value(wheels->gpio_backward, 1);
        break;

    case WHEELS_NONE:
        gpio_set_value(wheels->gpio_foreward, 0);
        gpio_set_value(wheels->gpio_backward, 0);
        break;

    default:
        return;
    }

    wheels->state = state;
}