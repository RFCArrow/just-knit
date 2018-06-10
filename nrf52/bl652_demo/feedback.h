#ifndef __FEEDBACK_H_
#define __FEEDBACK_H_

#include "nrf_gpio.h"

#define RED_PIN             NRF_GPIO_PIN_MAP(0,8)
#define BLUE_PIN            NRF_GPIO_PIN_MAP(0,7)
#define GREEN_PIN           NRF_GPIO_PIN_MAP(0,6)

#define FB_LED_PINS         RED_PIN|GREEN_PIN|BLUE_PIN
#define FB_OFF              0x0
#define FB_RED              RED_PIN
#define FB_GREEN            GREEN_PIN
#define FB_BLUE             BLUE_PIN
#define FB_YELLOW           FB_RED|FB_GREEN
#define FB_MAGENTA          FB_RED|FB_BLUE
#define FB_AQUA             FB_GREEN|FB_BLUE
#define FB_WHITE            FB_RED|FB_GREEN|FB_BLUE

#define MOTOR_PIN           NRF_GPIO_PIN_MAP(0,3)
#define BUZZ_PIN            NRF_GPIO_PIN_MAP(0,2)


typedef enum{
    FB_EVT_INIT,
    FB_EVT_ENTRY,
    FB_EVT_EXIT,
    FB_EVT_MOVEMENT_TIMEOUT,
    FB_EVT_ADVERTISING_TIMEOUT,
    FB_EVT_CHARGER_DISCONNECT,
    FB_EVT_LOW_POWER,
    FB_EVT_CHARGING_BEGIN,
    FB_EVT_CHARGING_COMPLETE,
    FB_EVT_MOVEMENT_DETECTED,
    FB_EVT_BLE_CONNECT,
    FB_EVT_BLE_DISCONNECT,
    FB_EVT_REQUEST_KNIT,
    FB_EVT_REQUEST_PURL,
    FB_EVT_KNIT_DETECTED,
    FB_EVT_PURL_DETECTED,
    FB_EVT_ANIMATION_END,
} fb_event_t;

uint32_t (* feedback_handler)(const fb_event_t event);

typedef uint32_t fb_state_t;
void fb_transition( fb_state_t (*new_state)(const fb_event_t event) );

fb_state_t fb_state_init(const fb_event_t event);
fb_state_t fb_state_sleep(const fb_event_t event);
fb_state_t fb_state_advertising(const fb_event_t event);
fb_state_t fb_state_connected(const fb_event_t event);
fb_state_t fb_state_knit(const fb_event_t event);
fb_state_t fb_state_purl(const fb_event_t event);
fb_state_t fb_state_correct(const fb_event_t event);
fb_state_t fb_state_incorrect(const fb_event_t event);
fb_state_t fb_state_low_power(const fb_event_t event);
fb_state_t fb_state_charging(const fb_event_t event);
fb_state_t fb_state_charged(const fb_event_t event);

typedef uint32_t fb_led_mask_t
void fb_set_led(fb_led_mask_t colour_mask);

void feedback_init(void);

#endif
