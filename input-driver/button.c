#include "button.h"

#include <linux/gpio.h>

int button_init(struct Button *button, unsigned int gpio, unsigned int keycode, irq_handler_t handler)
{
    char buff[50];
    int error;

    button->gpio = gpio;
    button->old_jiffie = 0;
    button->keycode = keycode;

    if (!gpio_is_valid(button->gpio))
    {
        return -EINVAL;
    }

    sprintf(buff, "GPIO %d", button->gpio);
    if (gpio_request(button->gpio, buff) < 0)
    {
        return -EIO;
    }

    gpio_direction_input(button->gpio);
    button->irq = gpio_to_irq(button->gpio);

    sprintf(buff, "button %d", button->gpio);
    if (request_irq(button->irq, handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, buff, (void*)button)) {
        error = -EBUSY;
        goto err_free_gpio;
    }

    return 0;

err_free_gpio:
    gpio_free(button->gpio);
    return error;
}

void button_free(struct Button *button)
{
    free_irq(button->irq, (void*)button);
    gpio_free(button->gpio);
}