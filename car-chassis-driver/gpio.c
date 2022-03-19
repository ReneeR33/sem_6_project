#include "gpio.h"

#include <linux/gpio.h>

int gpio_init(int number)
{
    if (!gpio_is_valid(number))
    {
        return -EINVAL;
    }

    char buff[50];

    sprintf(buff, "GPIO %d", number);
    if (gpio_request(number, buff) < 0)
    {
        return -EIO;
    }

    return 0;
}