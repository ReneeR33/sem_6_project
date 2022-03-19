#ifndef CAR_CHASSIS_DRIVER_H
#define CAR_CHASSIS_DRIVER_H

#define CAR_CHASSIS_IOC_MAGIC 99

#define CAR_CHASSIS_IOC_SL_WHEELS_STATE _IOW(CAR_CHASSIS_IOC_MAGIC, 1, WheelState)
#define CAR_CHASSIS_IOC_SR_WHEELS_STATE _IOW(CAR_CHASSIS_IOC_MAGIC, 2, WheelState)

typedef enum
{
    WHEELS_FOREWARD = 0,
    WHEELS_BACKWARD = 1,
    WHEELS_NONE     = 2
} WheelState;

#endif //CAR_CHASSIS_DRIVER_H