/**
 * Copyright (c) 2015 - 2018, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/** @file
 * @defgroup tw_sensor_example main.c
 * @{
 * @ingroup nrf_twi_example
 * @brief TWI Sensor Example main file.
 *
 * This file contains the source code for a sample application using TWI.
 *
 */

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "bno055.h"
#include "bno055_api_plugin.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

#define TWI_SCL_PIN         NRF_GPIO_PIN_MAP(0,27)
#define TWI_SDA_PIN         NRF_GPIO_PIN_MAP(0,22)

/* TWI instance. */
const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

struct bno055_t bno055;


/**
 * @brief UART initialization.
 */
void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_bno055_config = {
       .scl                = TWI_SCL_PIN,
       .sda                = TWI_SDA_PIN,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    // Initialise TWI with null event handler (blocking mode)
    err_code = nrf_drv_twi_init(&m_twi, &twi_bno055_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}

/**
 * @brief Function for reading data from temperature sensor.
 */
static void read_sensor_data()
{
    int16_t accel_datax;
    int16_t accel_datay;
    int16_t accel_dataz;
    bno055_read_accel_x(&accel_datax);
    bno055_read_accel_y(&accel_datay);
    bno055_read_accel_z(&accel_dataz);
    NRF_LOG_INFO("Acc:  x= %x",accel_datax);
    NRF_LOG_INFO("      y= %x",accel_datay);
    NRF_LOG_INFO("      z= %x",accel_dataz);
    NRF_LOG_FLUSH();
}




/**
 * @brief Function for main application entry.
 */
int main(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("\r\nTWI sensor example started.");
    NRF_LOG_FLUSH();
    twi_init();
    NRF_LOG_INFO("Connecting API");
    NRF_LOG_FLUSH();
    bno055_api_init(&bno055);
    //Might need to add 650ms delay before talking to BNO055
    nrf_delay_ms(650);
    NRF_LOG_INFO("Initialising BNO055");
    NRF_LOG_FLUSH();
    bno055_init(&bno055);
    BNO055_set_mode(BNO055_POWER_MODE_NORMAL,BNO055_OPERATION_MODE_ACCONLY);

    while (true)
    {
        nrf_delay_ms(500);
        read_sensor_data();
        NRF_LOG_PROCESS();
        NRF_LOG_FLUSH();
    }
}

/** @} */
