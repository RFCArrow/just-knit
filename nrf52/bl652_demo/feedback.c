#include "feedback.h"
#include "nrf_gpio.h"


void feedback_init(){
    nrf_gpio_cfg_output(MOTOR_PIN);
    nrf_gpio_pin_clear(MOTOR_PIN);

    nrf_gpio_cfg_output(BUZZ_PIN);
    nrf_gpio_pin_clear(BUZZ_PIN);

    
    nrf_gpio_cfg_output(RED_PIN);
    nrf_gpio_pin_clear(RED_PIN);

    
    nrf_gpio_cfg_output(BLUE_PIN);
    nrf_gpio_pin_clear(BLUE_PIN);


    nrf_gpio_cfg_output(GREEN_PIN);
    nrf_gpio_pin_clear(GREEN_PIN);
}
