#include "imu.h"

#include "bno055_api_plugin.h"


void imu_init(){
    BNO055_api_init(&bno055);
    //TODO: reset bno055 incase it has jammed up
    //Ensure that 650ms passes before this call
    nrf_delay_ms(650);
    bno055_init(&bno055);
    bno055_set_power_mode(IMU_POWER_MODE);
    bno055_set_operation_mode(IMU_OPERATION_MODE);
}

void imu_configure_interrupts(){
    //Must swap to config mode to configure
    bno055_set_operation_mode(BNO055_OPERATION_MODE_CONFIG);

    //Enable interrupts for X,Y,Z axes
    bno055_set_accel_any_motion_no_motion_axis_enable(BNO055_ACCEL_ANY_MOTION_NO_MOTION_X_AXIS, IMU_ENABLE);
    bno055_set_accel_any_motion_no_motion_axis_enable(BNO055_ACCEL_ANY_MOTION_NO_MOTION_Y_AXIS, IMU_ENABLE);
    bno055_set_accel_any_motion_no_motion_axis_enable(BNO055_ACCEL_ANY_MOTION_NO_MOTION_Z_AXIS, IMU_ENABLE);

    //Configure duration and thresholds for false positive reduction
    bno055_set_accel_any_motion_durn(IMU_MOTION_DURATION);
    bno055_set_accel_any_motion_thres(IMU_MOTION_THRESHOLD);
    bno055_set_accel_slow_no_motion_thres(IMU_NO_MOTION_THRESHOLD);
    bno055_set_accel_slow_no_motion_durn(IMU_NO_MOTION_DURATION);

    //Enable no-motion detection instead of slow motion
    bno055_set_accel_slow_no_motion_enable(IMU_ENABLE);

    //Kick off interrupts but don't expose any via INT PIN yet
    bno055_set_intr_accel_no_motion(IMU_ENABLE);
    bno055_set_intr_accel_any_motion(IMU_ENABLE);

    
    //Now return to operation mode
    bno055_set_operation_mode(IMU_OPERATION_MODE);

}

void imu_no_motion_interrupt(uint8_t enable_disable){
    bno055_set_operation_mode(BNO055_OPERATION_MODE_CONFIG);
	bno055_set_intr_mask_accel_no_motion(enable_disable);
    bno055_set_operation_mode(IMU_OPERATION_MODE);
}

void imu_any_motion_interrupt(uint8_t enable_disable){
    bno055_set_operation_mode(BNO055_OPERATION_MODE_CONFIG);
	bno055_set_intr_mask_accel_any_motion(enable_disable);
    bno055_set_operation_mode(IMU_OPERATION_MODE);
}

/**
 * @brief Function for reading data from imu.
 */
void imu_get_quaternions(uint8_t outBuffer[8])
{
    int16_t quaternion_dataw;
    int16_t quaternion_datax;
    int16_t quaternion_datay;
    int16_t quaternion_dataz;
    bno055_read_quaternion_w(&quaternion_dataw);
    bno055_read_quaternion_x(&quaternion_datax);
    bno055_read_quaternion_y(&quaternion_datay);
    bno055_read_quaternion_z(&quaternion_dataz);
    outBuffer[0] = quaternion_dataw & 0xff;
    outBuffer[1] = ((uint16_t)quaternion_dataw & 0xff00 ) >> 8;
    outBuffer[2] = quaternion_datax & 0xff;
    outBuffer[3] = ((uint16_t)quaternion_datax & 0xff00 ) >> 8;
    outBuffer[4] = quaternion_datay & 0xff;
    outBuffer[5] = ((uint16_t)quaternion_datay & 0xff00 ) >> 8;
    outBuffer[6] = quaternion_dataz & 0xff;
    outBuffer[7] = ((uint16_t)quaternion_dataz & 0xff00 ) >> 8;
    return;
}
