#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <asm/ioctl.h>
#include "wheels.h"

#define CAR_CHASSIS_IOC_MAGIC 99

#define CAR_CHASSIS_IOC_SL_WHEELS_STATE _IOW(CAR_CHASSIS_IOC_MAGIC, 1, WheelState)
#define CAR_CHASSIS_IOC_SR_WHEELS_STATE _IOW(CAR_CHASSIS_IOC_MAGIC, 2, WheelState)

#define GPIO_LEFT_FOREWARD 16
#define GPIO_LEFT_BACKWARD 19
#define GPIO_RIGHT_FOREWARD 20
#define GPIO_RIGHT_BACKWARD 21

static int car_chassis_driver_open(struct inode* pinode, struct file* pfile);
static int car_chassis_driver_release(struct inode* pinode, struct file* pfile);
static long car_chassis_driver_unlocked_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg);

static Wheels left_wheels;
static Wheels right_wheels;

struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .open           = car_chassis_driver_open,
    .release        = car_chassis_driver_release,
    .unlocked_ioctl = car_chassis_driver_unlocked_ioctl
};

static int car_chassis_driver_open(struct inode* pinode, struct file* pfile)
{
    pr_info("car-chassis-driver: open");

    return 0;
}

static int car_chassis_driver_release(struct inode* pinode, struct file* pfile)
{
    pr_info("car-chassis-driver: release");

    return 0;
}

static long car_chassis_driver_unlocked_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
    pr_info("car-chassis-driver: unlocked ioctl");

    if (arg < 0 || arg > 2)
    {
        pr_err("car-chassis-driver: received invalid ioctl argument %d", arg);
        return -EINVAL;
    }

    WheelState state = (WheelState)arg;

    switch (cmd)
    {
    case CAR_CHASSIS_IOC_SL_WHEELS_STATE:
        wheels_set_state(&left_wheels, state);
        break;

    case CAR_CHASSIS_IOC_SR_WHEELS_STATE:
        wheels_set_state(&right_wheels, state);
        break;
    
    default:
        pr_err("car-chassis-driver: invalid iocl command", arg);
        return -ENOTTY;
    }

    return 0;
}

static int __init car_chassis_driver_init(void)
{
    pr_info("car-chassis-driver: init");

    int error;

    if ((error = wheels_init(&left_wheels, GPIO_LEFT_FOREWARD, GPIO_LEFT_BACKWARD)) != 0)
    {
        return error;
    }
    if ((error = wheels_init(&right_wheels, GPIO_RIGHT_FOREWARD, GPIO_RIGHT_BACKWARD)) != 0)
    {
        goto err_free_left_wheels;
    }

    wheels_set_state(&left_wheels, WHEELS_FOREWARD);
    wheels_set_state(&right_wheels, WHEELS_FOREWARD);

    return 0;

err_free_left_wheels:
    wheels_free(&left_wheels);
    return error;
}

static void __exit car_chassis_driver_exit(void)
{
    pr_info("car-chassis-driver: exit");

    wheels_free(&left_wheels);
    wheels_free(&right_wheels);
}

module_init(car_chassis_driver_init);
module_exit(car_chassis_driver_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Renée Rulkens");