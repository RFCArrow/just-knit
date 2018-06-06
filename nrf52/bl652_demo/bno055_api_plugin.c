#include "bno055_api_plugin.h"
/*--------------------------------------------------------------------------*
*	The following API is used to map the I2C bus read, write, delay and
*	device address with global structure bno055_t
*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 *  By using bno055 the following structure parameter can be accessed
 *	Bus write function pointer: BNO055_WR_FUNC_PTR
 *	Bus read function pointer: BNO055_RD_FUNC_PTR
 *	Delay function pointer: delay_msec
 *	I2C address: dev_addr
 *--------------------------------------------------------------------------*/
void BNO055_api_init(struct bno055_t * p_bno055)
{
	p_bno055->bus_write = BNO055_I2C_bus_write;
	p_bno055->bus_read = BNO055_I2C_bus_read;
	p_bno055->delay_msec = BNO055_delay_msek;
	p_bno055->dev_addr = BNO055_I2C_ADDR1;
}

/************** I2C buffer length******/

#define	I2C_BUFFER_LEN 8
#define I2C0 5
/*-------------------------------------------------------------------*
*
*	This is a sample code for read and write the data by using I2C
*	Use either I2C  based on your need
*	The device address defined in the bno055.h file
*
*--------------------------------------------------------------------*/

/*	\Brief: The API is used as I2C bus write
 *	\Return : Status of the I2C write
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register,
 *   will data is going to be written
 *	\param reg_data : It is a value hold in the array,
 *		will be used for write the value into the register
 *	\param cnt : The no of byte of data to be write
 */
s8 BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	s32 BNO055_iERROR = 0;

    uint8_t write_data[cnt+1];

    if(sizeof(reg_data) < (sizeof(reg_data[0])*cnt)){
        NRF_LOG_ERROR("Data register too small for number of bytes specified");
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }

    ret_code_t err_code;

    write_data[0] = reg_addr;
    memcpy(write_data+sizeof(reg_data[0]),reg_data,cnt);

    // Select register for writing
    err_code = nrf_drv_twi_tx(&m_twi, dev_addr, &write_data[0], sizeof(write_data),false);

    if(err_code != NRF_SUCCESS){
        // Couldn't access register
        NRF_LOG_ERROR("Error: %x, could not select register %x",err_code, reg_addr);
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }
    /*
    // Read register
    err_code = nrf_drv_twi_tx(&m_twi, dev_addr, reg_data, cnt,false);
    
    if(err_code != NRF_SUCCESS){
        // Couldn't access register
        NRF_LOG_ERROR("Error: %x, could not write to register %x",err_code, reg_addr);
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }
    */
    
    APP_ERROR_CHECK(err_code);

	return (s8)BNO055_iERROR;
}

 /*	\Brief: The API is used as I2C bus read
 *	\Return : Status of the I2C read
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register,
 *  will data is going to be read
 *	\param reg_data : This data read from the sensor,
 *   which is hold in an array
 *	\param cnt : The no of byte of data to be read
 */
s8 BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	s32 BNO055_iERROR = 0;

    if(sizeof(reg_data) < (sizeof(reg_data[0])*cnt)){
        NRF_LOG_ERROR("Data register too small for number of bytes specified");
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }

    ret_code_t err_code;

    // Select register for reading
    err_code = nrf_drv_twi_tx(&m_twi, dev_addr, &reg_addr, sizeof(reg_addr),true);

    if(err_code != NRF_SUCCESS){
        // Couldn't access register
        NRF_LOG_ERROR("Error: %x, could not select register %x",err_code, reg_addr);
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }
    // Read register
    err_code = nrf_drv_twi_rx(&m_twi, dev_addr, reg_data, cnt);
    
    if(err_code != NRF_SUCCESS){
        // Couldn't access register
        NRF_LOG_ERROR("Error: %x, could not read register %x",err_code, reg_addr);
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }
    
    APP_ERROR_CHECK(err_code);

	return (s8)BNO055_iERROR;
}
/*	Brief : The delay routine
 *	\param : delay in ms
*/
void BNO055_delay_msek(u32 msek)
{
    nrf_delay_ms(msek);
}

/*
 * @brief Set mode of BNO055
 * param[in] power_mode 
 * param[in] operation_mode
 */
void BNO055_set_mode(uint8_t power_mode, uint8_t operation_mode){

    uint8_t observed_power_mode;
    uint8_t observed_operation_mode;

    NRF_LOG_INFO("Setting Power Mode to %x", power_mode);
    
    bno055_set_power_mode(power_mode);
    bno055_set_operation_mode(operation_mode);
    //nrf_delay_ms(500);
    bno055_get_power_mode(&observed_power_mode);

    if(observed_power_mode != power_mode){
        NRF_LOG_ERROR("Could not set Power Mode to %x, Power Mode currently: %x",power_mode, observed_power_mode);
    }
    else NRF_LOG_INFO("Power Mode set to %x", power_mode);

    NRF_LOG_INFO("Setting Operation Mode to %x", operation_mode);

    bno055_get_operation_mode(&observed_operation_mode);
    if(observed_operation_mode != operation_mode){
        NRF_LOG_ERROR("Could not set Operation Mode to %x, Operation Mode currently: %x",operation_mode, observed_operation_mode);
    }
    else NRF_LOG_INFO("Operation Mode set to %x", operation_mode);

    NRF_LOG_FLUSH();
}


/**
 * @brief Function for reading data from imu.
 */
void BNO055_get_quaternions(uint8_t outBuffer[8])
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



