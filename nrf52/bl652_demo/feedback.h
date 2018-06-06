#ifndef __FEEDBACK_H_
#define __FEEDBACK_H_

#include "nrf_gpio.h"


#define MOTOR_PIN           NRF_GPIO_PIN_MAP(0,3)
#define BUZZ_PIN            NRF_GPIO_PIN_MAP(0,2)



void feedback_init(void);

#endif
