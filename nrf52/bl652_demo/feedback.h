#ifndef __FEEDBACK_H_
#define __FEEDBACK_H_

#include "nrf_gpio.h"

#define RED_PIN             NRF_GPIO_PIN_MAP(0,8)
#define BLUE_PIN            NRF_GPIO_PIN_MAP(0,7)
#define GREEN_PIN           NRF_GPIO_PIN_MAP(0,6)

#define MOTOR_PIN           NRF_GPIO_PIN_MAP(0,3)
#define BUZZ_PIN            NRF_GPIO_PIN_MAP(0,2)




void feedback_init(void);

#endif
