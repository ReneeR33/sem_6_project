#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/i2c.h>
#include <asm/ioctl.h>

#define I2C_BUS_AVAILABLE 1
#define SLAVE_DEVICE_NAME "EXT_CAM"
#define CAM_SLAVE_ADDR 0x21

static dev_t devnr;
static struct cdev device;

static struct i2c_adapter *etx_i2c_adapter = NULL;
static struct i2c_client *etx_i2c_client_led = NULL;

static struct i2c_board_info led_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, CAM_SLAVE_ADDR)
};
 
static ssize_t i2c_driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

static int etx_cam_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int etx_cam_remove(struct i2c_client *client);

struct file_operations fops = {
    .owner          = THIS_MODULE,
    .write          = i2c_driver_write
};

static const struct i2c_device_id etx_cam_id[] = {
    { SLAVE_DEVICE_NAME, 0 },
    {}
};

MODULE_DEVICE_TABLE(i2c, etx_cam_id);

static struct i2c_driver etx_led_driver = {
    .driver = {
        .name  = SLAVE_DEVICE_NAME,
        .owner = THIS_MODULE
    },
    .probe    = etx_cam_probe,
    .remove   = etx_cam_remove,
    .id_table = etx_cam_id
};

static ssize_t i2c_driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
    /*char user_buf;
    if (copy_from_user(&user_buf, buff, 1) != 0)
    {
        pr_err("camera-driver: failed to copy user data\n");
        return 0;
    }

    pr_info("camera-driver: received %c\n", user_buf);
    
    uint8_t send_buf = 0x0;
    if (user_buf != '0')
    {
        send_buf = 0x1;
    }

    int ret = i2c_master_send(etx_i2c_client_led, &send_buf, 1);
    if (ret < 0)
    {
        pr_err("camera-driver: error sending i2c data, returned error code %d\n", ret);
    }*/

    return count;
}

static int etx_cam_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    pr_info("camera-driver: i2c cam probe\n");
    return 0;
}

static int etx_cam_remove(struct i2c_client *client)
{
    pr_info("camera-driver: i2c cam remove\n");
    return 0;
}

static int __init i2c_driver_init(void)
{
    pr_info("camera-driver: init\n");

    int error;

    if ((error = alloc_chrdev_region(&devnr, 0, 1, "camera-driver")) != 0)
    {
        pr_err("camera-driver: can't get major\n");
        return error;
    }

    cdev_init(&device, &fops);
    device.owner = THIS_MODULE;

    if ((error = cdev_add(&device, devnr, 1)) != 0)
    {
        pr_err("camera-driver: couldn't add device\n");
        goto err_unreg_chrdev_region;
    }

    etx_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
    if (etx_i2c_adapter == NULL)
    {
        pr_err("camera-driver: couldn't obtain i2c adapter\n");
        error = -ENODEV;
        goto err_cdev_del;
    }

    etx_i2c_client_led = i2c_new_client_device(etx_i2c_adapter, &led_i2c_board_info);
    if (etx_i2c_client_led == NULL)
    {
        pr_err("camera-driver: couldn't create i2c client device\n");
        error = -ENODEV;
        goto err_cdev_del;
    }

    if ((error = i2c_add_driver(&etx_led_driver)) != 0)
    {
        pr_err("camera-driver: error while adding i2c driver\n");
        goto err_unreg_i2c_device;
    }

    return 0;

err_unreg_i2c_device:
    i2c_unregister_device(etx_i2c_client_led);
err_cdev_del:
    cdev_del(&device);
err_unreg_chrdev_region:
    unregister_chrdev_region(devnr, 1);

    return error;
}

static void __exit i2c_driver_exit(void)
{
    pr_info("camera-driver: exit\n");

    i2c_del_driver(&etx_led_driver);
    i2c_unregister_device(etx_i2c_client_led);
    cdev_del(&device);
    unregister_chrdev_region(devnr, 1);
}

module_init(i2c_driver_init);
module_exit(i2c_driver_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Renée Rulkens");