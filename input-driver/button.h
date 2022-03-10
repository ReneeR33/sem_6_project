#ifndef BUTTON_H
#define BUTTON_H

#include <linux/interrupt.h>

struct Button
{
    unsigned int gpio;
    unsigned int keycode;
    unsigned int irq;
};

int button_init(struct Button *button, unsigned int gpio, unsigned int keycode, irq_handler_t handler);
void button_free(struct Button *button);

#endif //BUTTON_H