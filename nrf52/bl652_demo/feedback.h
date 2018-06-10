#ifndef __FEEDBACK_H_
#define __FEEDBACK_H_

#include "nrf_gpio.h"

#define RED_PIN             NRF_GPIO_PIN_MAP(0,8)
#define BLUE_PIN            NRF_GPIO_PIN_MAP(0,7)
#define GREEN_PIN           NRF_GPIO_PIN_MAP(0,6)

#define FB_OFF              0x0
#define FB_RED              0x1<<8
#define FB_GREEN            0x1<<6
#define FB_BLUE             0x1<<7
#define FB_YELLOW           FB_RED|FB_GREEN
#define FB_MAGENTA          FB_RED|FB_BLUE
#define FB_AQUA             FB_GREEN|FB_BLUE
#define FB_WHITE            FB_RED|FB_GREEN|FB_BLUE
#define FB_LED_PINS         FB_RED|FB_GREEN|FB_BLUE

#define MOTOR_PIN           NRF_GPIO_PIN_MAP(0,3)
#define BUZZ_PIN            NRF_GPIO_PIN_MAP(0,2)

#define FB_CHARGING_SOFTBLINK_PARAMS        \
        {                                   \
            .active_high        = true,     \
            .duty_cycle_max     = 200,      \
            .duty_cycle_min     = 5,        \
            .duty_cycle_step    = 1,        \
            .off_time_ticks     = 255,      \
            .on_time_ticks      = 255,      \
            .leds_pin_bm        = FB_WHITE, \
            .p_leds_port        = NRF_GPIO  \
        }

#define FB_ADVERTISING_SOFTBLINK_PARAMS     \
        {                                   \
            .active_high        = true,     \
            .duty_cycle_max     = 200,      \
            .duty_cycle_min     = 5,        \
            .duty_cycle_step    = 1,        \
            .off_time_ticks     = 255,      \
            .on_time_ticks      = 255,      \
            .leds_pin_bm        = FB_BLUE,  \
            .p_leds_port        = NRF_GPIO  \
        }

#define FB_REQ_KNIT_SOFTBLINK_PARAMS        \
        {                                   \
            .active_high        = true,     \
            .duty_cycle_max     = 100,      \
            .duty_cycle_min     = 5,        \
            .duty_cycle_step    = 10,        \
            .off_time_ticks     = 127,      \
            .on_time_ticks      = 127,      \
            .leds_pin_bm        = FB_GREEN, \
            .p_leds_port        = NRF_GPIO  \
        }

#define FB_REQ_PURL_SOFTBLINK_PARAMS        \
        {                                   \
            .active_high        = true,     \
            .duty_cycle_max     = 100,      \
            .duty_cycle_min     = 5,        \
            .duty_cycle_step    = 10,       \
            .off_time_ticks     = 127,      \
            .on_time_ticks      = 127,      \
            .leds_pin_bm        = FB_RED, \
            .p_leds_port        = NRF_GPIO  \
        }

typedef uint32_t fb_led_mask_t;
void fb_set_led(fb_led_mask_t colour_mask);

void feedback_init(void);


void fb_start_softblink_purl(void);
void fb_start_softblink_knit(void);
void fb_start_softblink_charging(void);
void fb_start_softblink_advertising(void);
void fb_stop_softblink(void);


#endif
