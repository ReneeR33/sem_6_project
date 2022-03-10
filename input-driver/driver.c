#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>

#include <asm/irq.h>
#include <asm/io.h>

#include "button.h"

#define BTN_COUNT 4

#define GPIO_BTN_0 21
#define GPIO_BTN_1 20
#define GPIO_BTN_2 7
#define GPIO_BTN_3 8

static struct input_dev *button_dev;
static struct Button buttons[BTN_COUNT];

static irqreturn_t button_interrupt(int irq, void *data)
{
    struct Button *button = (struct Button*)data;

    int pressed = gpio_get_value(button->gpio);                    

    input_report_key(button_dev, button->keycode, pressed);        
    input_sync(button_dev);

    return IRQ_HANDLED;                                                                          
}

static int __init input_driver_init(void)
{
    int error = -1;

    button_dev = input_allocate_device();
    if (!button_dev) {
        pr_err("input-driver: Not enough memory\n");
        error = -ENOMEM;
        return error;
    }

    button_dev->evbit[0] = BIT_MASK(EV_KEY);
    
    button_dev->keybit[BIT_WORD(BTN_0)] |= BIT_MASK(BTN_0);
    button_dev->keybit[BIT_WORD(BTN_1)] |= BIT_MASK(BTN_1);
    button_dev->keybit[BIT_WORD(BTN_2)] |= BIT_MASK(BTN_2);
    button_dev->keybit[BIT_WORD(BTN_3)] |= BIT_MASK(BTN_3);

    error = input_register_device(button_dev);
    if (error) {
        pr_err("input-driver: Failed to register device\n");
        goto err_free_dev;
    }

    error = button_init(&buttons[0], GPIO_BTN_0, BTN_0, button_interrupt);
    if (error != 0)
    {
        goto err_free_dev;
    }
    error = button_init(&buttons[1], GPIO_BTN_1, BTN_1, button_interrupt);
    if (error != 0)
    {
        goto err_free_button_0;
    }
    error = button_init(&buttons[2], GPIO_BTN_2, BTN_2, button_interrupt);
    if (error != 0)
    {
        goto err_free_button_1;
    }
    error = button_init(&buttons[3], GPIO_BTN_3, BTN_3, button_interrupt);
    if (error != 0)
    {
        goto err_free_button_2;
    }

    pr_info("input-driver: initialized");

    return 0;

err_free_button_2:
    button_free(&buttons[2]);
err_free_button_1:
    button_free(&buttons[1]);
err_free_button_0:
    button_free(&buttons[0]);
err_free_dev:
    input_free_device(button_dev);
    return error;
}

static void __exit input_driver_exit(void)
{
    input_unregister_device(button_dev);
    for (int i = 0; i < BTN_COUNT; i++)
    {
        button_free(&buttons[i]);
    }

    pr_info("input-driver: removed\n");
}

module_init(input_driver_init);
module_exit(input_driver_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Renée Rulkens");