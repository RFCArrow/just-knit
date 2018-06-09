#include "feedback.h"
#include "nrf_gpio.h"


void feedback_init(){
    nrf_gpio_cfg_output(MOTOR_PIN);
    nrf_gpio_pin_clear(MOTOR_PIN);

    nrf_gpio_cfg_output(BUZZ_PIN);
    nrf_gpio_pin_clear(BUZZ_PIN);

   //Initialise LEDs 
    nrf_gpio_cfg_output(RED_PIN);
    nrf_gpio_pin_clear(RED_PIN);
    nrf_gpio_cfg_output(BLUE_PIN);
    nrf_gpio_pin_clear(BLUE_PIN);
    nrf_gpio_cfg_output(GREEN_PIN);
    nrf_gpio_pin_clear(GREEN_PIN);

    //Initial feedback fsm
    feedback_handler = fb_state_init;
}

//Initial State
fb_state_t fb_state_init(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_MOVEMENT_DETECTED:
            fb_transition(&fb_state_advertising);
        break;
        case default:
            //do nothing
        break;
    }
    return err_code;
}

fb_state_t fb_state_sleeping(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //turn off all lights
        break;
        case FB_EVT_EXIT:
            //no need to do anything
        break;
        case FB_EVT_MOVEMENT_DETECTED:
            fb_transition(&fb_state_advertising);
        break;
        case FB_EVT_CHARGING_BEGIN:
            fb_transition(&fb_state_charging);
        break;
        default:
            //no need to do anything
        break;
    }
    return err_code;
}

fb_state_t fb_state_advertising(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //begin softblink
        break;
        case FB_EVT_EXIT:
            //stop softblink
        break;
        case FB_EVT_BLE_CONNECT:
            fb_transition(&fb_state_connected);
        break;
        case FB_EVT_ADVERTISING_TIMEOUT:
            fb_transition(&fb_state_sleep);
        break;
        case FB_EVT_CHARGING_BEGIN:
            fb_transition(&fb_state_charging);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}
uint32_t fb_state_connected(const fb_event_t event);
uint32_t fb_state_knit(const fb_event_t event);
uint32_t fb_state_purl(const fb_event_t event);
uint32_t fb_state_correct(const fb_event_t event);
uint32_t fb_state_incorrect(const fb_event_t event);
uint32_t fb_state_charging(const fb_event_t event);
uint32_t fb_state_charged(const fb_event_t event);
