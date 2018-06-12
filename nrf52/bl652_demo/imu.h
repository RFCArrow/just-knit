#ifndef __IMU_H_
#define __IMU_H_

#include <stdint.h>
#include "bno055.h"

#define IMU_POWER_MODE          BNO055_POWER_MODE_NORMAL          
#define IMU_OPERATION_MODE      BNO055_OPERATION_MODE_NDOF

//Set threshold of 4 LSB to reduce false positive
#define IMU_MOTION_THRESHOLD       0x04
//Set duration 2 samples(N+1 samples) to reduce false positive
#define IMU_MOTION_DURATION        0x01
#define IMU_NO_MOTION_THRESHOLD    0x05
#define IMU_NO_MOTION_DURATION     0x01
#define IMU_ENABLE                 0x01
#define IMU_DISABLE                0x00


struct bno055_t bno055;

void imu_init();

void imu_configure_interrupts();

void imu_no_motion_interrupt(uint8_t enable_disable);
void imu_any_motion_interrupt(uint8_t enable_disable);

void imu_get_quaternions(uint8_t quatBuffer[8]);





#endif
