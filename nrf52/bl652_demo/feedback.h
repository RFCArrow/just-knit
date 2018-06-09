#ifndef __FEEDBACK_H_
#define __FEEDBACK_H_

#include "nrf_gpio.h"

#define RED_PIN             NRF_GPIO_PIN_MAP(0,8)
#define BLUE_PIN            NRF_GPIO_PIN_MAP(0,7)
#define GREEN_PIN           NRF_GPIO_PIN_MAP(0,6)

#define MOTOR_PIN           NRF_GPIO_PIN_MAP(0,3)
#define BUZZ_PIN            NRF_GPIO_PIN_MAP(0,2)


typedef enum{
    FB_EVT_INIT,
    FB_EVT_ENTRY,
    FB_EVT_EXIT,
    FB_EVT_MOVEMENT_TIMEOUT,
    FB_EVT_CHARGER_DISCONNECT,
    FB_EVT_CHARGING_BEGIN,
    FB_EVT_CHARGING_COMPLETE,
    FB_EVT_MOVEMENT_DETECTED,
    FB_EVT_BLE_CONNECT,
    FB_EVT_BLE_DISCONNECT,
    FB_EVT_KNIT_REQUEST,
    FB_EVT_PURL_REQUEST,
    FB_EVT_CORRECT_STITCH,
    FB_EVT_INCORRECT_STITCH,
} fb_event_t;

uint32_t (* feedback_handler)(const fb_event_t event);

typedef uint32_t fb_state_t;
inline void fb_transition( fb_state_t (*new_state)(const fb_event_t event) ){
    feedback_handler(FB_EVT_EXIT);
    feedback_handler = new_state;
    feedback_handler(FB_EVT_ENTRY);
}

fb_state_t fb_state_init(const fb_event_t event);
fb_state_t fb_state_sleeping(const fb_event_t event);
fb_state_t fb_state_advertising(const fb_event_t event);
fb_state_t fb_state_connected(const fb_event_t event);
fb_state_t fb_state_knit(const fb_event_t event);
fb_state_t fb_state_purl(const fb_event_t event);
fb_state_t fb_state_correct(const fb_event_t event);
fb_state_t fb_state_incorrect(const fb_event_t event);
fb_state_t fb_state_charging(const fb_event_t event);
fb_state_t fb_state_charged(const fb_event_t event);



void feedback_init(void);

#endif
