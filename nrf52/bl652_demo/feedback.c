#include "feedback.h"


void feedback_init(){
    nrf_gpio_cfg_output(MOTOR_PIN);
    nrf_gpio_pin_clear(MOTOR_PIN);

    nrf_gpio_cfg_output(BUZZ_PIN);
    nrf_gpio_pin_clear(BUZZ_PIN);

}
