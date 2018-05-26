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
 *
 * @defgroup led_softblink_example_main main.c
 * @{
 * @ingroup led_softblink_example
 * @brief LED Soft Blink Example Application main file.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "led_softblink.h"
#include "app_error.h"
#include "sdk_errors.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "app_util_platform.h"

#define LED_SB_CHARGING_ACTIVE_HIGH         true
#define LED_SB_CHARGING_DUTY_CYCLE_MAX      (uint8_t)200
#define LED_SB_CHARGING_DUTY_CYCLE_MIN      (uint8_t)5
#define LED_SB_CHARGING_DUTY_CYCLE_STEP     (uint8_t)1
#define LED_SB_CHARGING_OFF_TIME_TICKS      (uint32_t)255
#define LED_SB_CHARGING_ON_TIME_TICKS       (uint32_t)255
#define LED_SB_CHARGING_LEDS_PIN_BM         (uint32_t)1<<7
#define LED_SB_CHARGING_LEDS_PORT           LED_SB_INIT_PARAMS_LEDS_PORT

#define LED_RED                             NRF_GPIO_PIN_MAP(0,8)
#define LED_BLUE                             NRF_GPIO_PIN_MAP(0,7)
#define LED_GREEN                            NRF_GPIO_PIN_MAP(0,6)


/**
 * @brief Function for starting lfclk needed by APP_TIMER.
 */
static void lfclk_init(void)
{
    uint32_t err_code;
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}

static void leds_init(void){
    nrf_gpio_cfg_output(LED_RED);
    nrf_gpio_cfg_output(LED_BLUE);
    nrf_gpio_cfg_output(LED_GREEN);

    nrf_gpio_pin_clear(LED_RED);
    nrf_gpio_pin_clear(LED_BLUE);
    nrf_gpio_pin_clear(LED_GREEN);
    
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    ret_code_t err_code;

    lfclk_init();

    leds_init();

    // Start APP_TIMER to generate timeouts.
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    const led_sb_init_params_t led_sb_init_param = {                    \
        .active_high        = LED_SB_CHARGING_ACTIVE_HIGH,           \
        .duty_cycle_max     = LED_SB_CHARGING_DUTY_CYCLE_MAX,        \
        .duty_cycle_min     = LED_SB_CHARGING_DUTY_CYCLE_MIN,        \
        .duty_cycle_step    = LED_SB_CHARGING_DUTY_CYCLE_STEP,       \
        .off_time_ticks     = LED_SB_CHARGING_OFF_TIME_TICKS,        \
        .on_time_ticks      = LED_SB_CHARGING_ON_TIME_TICKS,         \
        .leds_pin_bm        = LED_SB_CHARGING_LEDS_PIN_BM,     \
        .p_leds_port        = LED_SB_CHARGING_LEDS_PORT              \
    };




    err_code = led_softblink_init(&led_sb_init_param);
    APP_ERROR_CHECK(err_code);

    err_code = led_softblink_start(LED_SB_CHARGING_LEDS_PIN_BM);
    APP_ERROR_CHECK(err_code);

    while (true)
    {
        __WFE();
    }
}

/** @} */
