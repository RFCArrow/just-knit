/*
****************************************************************************
* Copyright (C) 2015 - 2016 Bosch Sensortec GmbH
*
* bno055_support.c
* Date: 2016/03/14
* Revision: 1.0.4 $
*
* Usage: Sensor Driver support file for BNO055 sensor
*
****************************************************************************
* License:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
*   Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
*   Neither the name of the copyright holder nor the names of the
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
* OR CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
* The information provided is believed to be accurate and reliable.
* The copyright holder assumes no responsibility
* for the consequences of use
* of such information nor for any infringement of patents or
* other rights of third parties which may result from its use.
* No license is granted by implication or otherwise under any patent or
* patent rights of the copyright holder.
**************************************************************************/
/*---------------------------------------------------------------------------*
 Includes
*---------------------------------------------------------------------------*/
#include "bno055.h"

/*----------------------------------------------------------------------------*
 *  The following APIs are used for reading and writing of
 *	sensor data using I2C communication
*----------------------------------------------------------------------------*/

/********************End of I2C APIs declarations***********************/
/* This API is an example for reading sensor data
 *	\param: None
 *	\return: communication result
 */
s32 bno055_data_readout_template(void);
/*----------------------------------------------------------------------------*
 *  struct bno055_t parameters can be accessed by using BNO055
 *	BNO055_t having the following parameters
 *	Bus write function pointer: BNO055_WR_FUNC_PTR
 *	Bus read function pointer: BNO055_RD_FUNC_PTR
 *	Burst read function pointer: BNO055_BRD_FUNC_PTR
 *	Delay function pointer: delay_msec
 *	I2C address: dev_addr
 *	Chip id of the sensor: chip_id
*---------------------------------------------------------------------------*/
struct bno055_t bno055;
/* This API is an example for reading sensor data
 *	\param: None
 *	\return: communication result
 */
s32 bno055_data_readout_template(void)
{
	/* Variable used to return value of
	communication routine*/
	s32 comres = BNO055_ERROR;
	/* variable used to set the power mode of the sensor*/
	u8 power_mode = BNO055_INIT_VALUE;

	/************read raw quaternion data**************/
	/* variable used to read the quaternion w data */
	s16 quaternion_data_w = BNO055_INIT_VALUE;
	/* variable used to read the quaternion x data */
	s16 quaternion_data_x = BNO055_INIT_VALUE;
	/* variable used to read the quaternion y data */
	s16 quaternion_data_y = BNO055_INIT_VALUE;
	/* variable used to read the quaternion z data */
	s16 quaternion_data_z = BNO055_INIT_VALUE;
	/* structure used to read the quaternion wxyz data */
	struct bno055_quaternion_t quaternion_wxyz;

	/*****************read gyro converted data************************/
	/* variable used to read the gyro x data output as dps or rps */
	double d_gyro_datax = BNO055_INIT_VALUE;
	/* variable used to read the gyro y data output as dps or rps */
	double d_gyro_datay = BNO055_INIT_VALUE;
	/* variable used to read the gyro z data output as dps or rps */
	double d_gyro_dataz = BNO055_INIT_VALUE;
	/* structure used to read the gyro xyz data output as dps or rps */
	struct bno055_gyro_double_t d_gyro_xyz;

	/*********read linear acceleration converted data**********/
	/* variable used to read the linear accel x data output as m/s2*/
	double d_linear_accel_datax = BNO055_INIT_VALUE;
	/* variable used to read the linear accel y data output as m/s2*/
	double d_linear_accel_datay = BNO055_INIT_VALUE;
	/* variable used to read the linear accel z data output as m/s2*/
	double d_linear_accel_dataz = BNO055_INIT_VALUE;
	/* structure used to read the linear accel xyz data output as m/s2*/
	struct bno055_linear_accel_double_t d_linear_accel_xyz;

 *********************** START INITIALIZATION ************************
 *--------------------------------------------------------------------------*/
/*	Based on the user need configure I2C interface.
 *	It is example code to explain how to use the bno055 API*/
	I2C_routine();
/*--------------------------------------------------------------------------*
 *  This API used to assign the value/reference of
 *	the following parameters
 *	I2C address
 *	Bus Write
 *	Bus read
 *	Chip id
 *	Page id
 *	Accel revision id
 *	Mag revision id
 *	Gyro revision id
 *	Boot loader revision id
 *	Software revision id
 *-------------------------------------------------------------------------*/
	comres = bno055_init(&bno055);

/*	For initializing the BNO sensor it is required to the operation mode
	of the sensor as NORMAL
	Normal mode can set from the register
	Page - page0
	register - 0x3E
	bit positions - 0 and 1*/
	power_mode = BNO055_POWER_MODE_NORMAL;
	/* set the power mode as NORMAL*/
	comres += bno055_set_power_mode(power_mode);
/*----------------------------------------------------------------*
************************* END INITIALIZATION *************************
*-----------------------------------------------------------------*/

/************************* START READ RAW SENSOR DATA****************/

/*	Using BNO055 sensor we can read the following sensor data and
	virtual sensor data
	Sensor data:
		Accel
		Mag
		Gyro
	Virtual sensor data
		Euler
		Quaternion
		Linear acceleration
		Gravity sensor */
/*	For reading sensor raw data it is required to set the
	operation modes of the sensor
	operation mode can set from the register
	page - page0
	register - 0x3D
	bit - 0 to 3
	for sensor data read following operation mode have to set
	 * SENSOR MODE
		*0x01 - BNO055_OPERATION_MODE_ACCONLY
		*0x02 - BNO055_OPERATION_MODE_MAGONLY
		*0x03 - BNO055_OPERATION_MODE_GYRONLY
		*0x04 - BNO055_OPERATION_MODE_ACCMAG
		*0x05 - BNO055_OPERATION_MODE_ACCGYRO
		*0x06 - BNO055_OPERATION_MODE_MAGGYRO
		*0x07 - BNO055_OPERATION_MODE_AMG
		based on the user need configure the operation mode*/
	comres += bno055_set_operation_mode(BNO055_OPERATION_MODE_AMG);
/*	Raw accel X, Y and Z data can read from the register
	page - page 0
	register - 0x08 to 0x0D*/
	comres += bno055_read_accel_x(&accel_datax);
	comres += bno055_read_accel_y(&accel_datay);
	comres += bno055_read_accel_z(&accel_dataz);
	comres += bno055_read_accel_xyz(&accel_xyz);
/*	Raw mag X, Y and Z data can read from the register
	page - page 0
	register - 0x0E to 0x13*/
	comres += bno055_read_mag_x(&mag_datax);
	comres += bno055_read_mag_y(&mag_datay);
	comres += bno055_read_mag_z(&mag_dataz);
	comres += bno055_read_mag_xyz(&mag_xyz);
/*	Raw gyro X, Y and Z data can read from the register
	page - page 0
	register - 0x14 to 0x19*/
	comres += bno055_read_gyro_x(&gyro_datax);
	comres += bno055_read_gyro_y(&gyro_datay);
	comres += bno055_read_gyro_z(&gyro_dataz);
	comres += bno055_read_gyro_xyz(&gyro_xyz);

/************************* END READ RAW SENSOR DATA****************/

/************************* START READ RAW FUSION DATA ********
	For reading fusion data it is required to set the
	operation modes of the sensor
	operation mode can set from the register
	page - page0
	register - 0x3D
	bit - 0 to 3
	for sensor data read following operation mode have to set
	*FUSION MODE
		*0x08 - BNO055_OPERATION_MODE_IMUPLUS
		*0x09 - BNO055_OPERATION_MODE_COMPASS
		*0x0A - BNO055_OPERATION_MODE_M4G
		*0x0B - BNO055_OPERATION_MODE_NDOF_FMC_OFF
		*0x0C - BNO055_OPERATION_MODE_NDOF
		based on the user need configure the operation mode*/
	comres += bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
/*	Raw Quaternion W, X, Y and Z data can read from the register
	page - page 0
	register - 0x20 to 0x27 */
	comres += bno055_read_quaternion_w(&quaternion_data_w);
	comres += bno055_read_quaternion_x(&quaternion_data_x);
	comres += bno055_read_quaternion_y(&quaternion_data_y);
	comres += bno055_read_quaternion_z(&quaternion_data_z);
	comres += bno055_read_quaternion_wxyz(&quaternion_wxyz);
/*	Raw Linear accel X, Y and Z data can read from the register
	page - page 0
	register - 0x28 to 0x2D */
	comres += bno055_read_linear_accel_x(&linear_accel_data_x);
	comres += bno055_read_linear_accel_y(&linear_accel_data_y);
	comres += bno055_read_linear_accel_z(&linear_accel_data_z);
	comres += bno055_read_linear_accel_xyz(&linear_acce_xyz);
/************************* END READ RAW FUSION DATA  ************/

/******************START READ CONVERTED SENSOR DATA****************/
/*	API used to read accel data output as double  - m/s2 and mg
	float functions also available in the BNO055 API */
	comres += bno055_convert_double_accel_x_msq(&d_accel_datax);
	comres += bno055_convert_double_accel_x_mg(&d_accel_datax);
	comres += bno055_convert_double_accel_y_msq(&d_accel_datay);
	comres += bno055_convert_double_accel_y_mg(&d_accel_datay);
	comres += bno055_convert_double_accel_z_msq(&d_accel_dataz);
	comres += bno055_convert_double_accel_z_mg(&d_accel_dataz);
	comres += bno055_convert_double_accel_xyz_msq(&d_accel_xyz);
	comres += bno055_convert_double_accel_xyz_mg(&d_accel_xyz);

/*	API used to read gyro data output as double  - dps and rps
	float functions also available in the BNO055 API */
	comres += bno055_convert_double_gyro_x_dps(&d_gyro_datax);
	comres += bno055_convert_double_gyro_y_dps(&d_gyro_datay);
	comres += bno055_convert_double_gyro_z_dps(&d_gyro_dataz);
	comres += bno055_convert_double_gyro_x_rps(&d_gyro_datax);
	comres += bno055_convert_double_gyro_y_rps(&d_gyro_datay);
	comres += bno055_convert_double_gyro_z_rps(&d_gyro_dataz);
	comres += bno055_convert_double_gyro_xyz_dps(&d_gyro_xyz);
	comres += bno055_convert_double_gyro_xyz_rps(&d_gyro_xyz);

/*	API used to read Linear acceleration data output as m/s2
	float functions also available in the BNO055 API */
	comres += bno055_convert_double_linear_accel_x_msq(
	&d_linear_accel_datax);
	comres += bno055_convert_double_linear_accel_y_msq(
	&d_linear_accel_datay);
	comres += bno055_convert_double_linear_accel_z_msq(
	&d_linear_accel_dataz);
	comres += bno055_convert_double_linear_accel_xyz_msq(
	&d_linear_accel_xyz);

************************* START DE-INITIALIZATION ***********************
*-------------------------------------------------------------------------*/
/*	For de - initializing the BNO sensor it is required
	to the operation mode of the sensor as SUSPEND
	Suspend mode can set from the register
	Page - page0
	register - 0x3E
	bit positions - 0 and 1*/
	power_mode = BNO055_POWER_MODE_SUSPEND;
	/* set the power mode as SUSPEND*/
	comres += bno055_set_power_mode(power_mode);

/*---------------------------------------------------------------------*
************************* END DE-INITIALIZATION **********************
*---------------------------------------------------------------------*/
return comres;
}

