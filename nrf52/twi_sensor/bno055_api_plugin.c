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
void bno055_api_init(struct bno055_t * p_bno055)
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

    if(sizeof(reg_data) < (sizeof(reg_data[0])*cnt)){
        NRF_LOG_ERROR("Data register too small for number of bytes specified");
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }

    ret_code_t err_code;

    // Select register for writing
    err_code = nrf_drv_twi_tx(&m_twi, dev_addr, &reg_addr, sizeof(reg_addr),true);

    if(err_code != NRF_SUCCESS){
        // Couldn't access register
        NRF_LOG_ERROR("Error: %x, could not select register %x",err_code, reg_addr);
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }
    // Read register
    err_code = nrf_drv_twi_tx(&m_twi, dev_addr, reg_data, cnt,false);
    
    if(err_code != NRF_SUCCESS){
        // Couldn't access register
        NRF_LOG_ERROR("Error: %x, could not write to register %x",err_code, reg_addr);
        NRF_LOG_FLUSH();
        BNO055_iERROR = -1;
        return (s8)BNO055_iERROR;
    }
    
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
    
    bno055_set_power_mode(power_mode);
    bno055_set_operation_mode(operation_mode);
}





