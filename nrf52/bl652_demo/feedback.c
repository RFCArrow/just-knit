#include "feedback.h"
#include "nrf_gpio.h"


void feedback_init(){
    nrf_gpio_cfg_output(MOTOR_PIN);
    nrf_gpio_pin_clear(MOTOR_PIN);

    nrf_gpio_cfg_output(BUZZ_PIN);
    nrf_gpio_pin_clear(BUZZ_PIN);

   //Initialise LEDs 
    nrf_gpio_cfg_output(RED_PIN);
    nrf_gpio_cfg_output(BLUE_PIN);
    nrf_gpio_cfg_output(GREEN_PIN);

    fb_set_led(FB_OFF);

    //Initial feedback fsm
    feedback_handler = fb_state_init;
}

void fb_transition( fb_state_t (*new_state)(const fb_event_t event) ){
    feedback_handler(FB_EVT_EXIT);
    feedback_handler = new_state;
    feedback_handler(FB_EVT_ENTRY);
}

//Initial State
fb_state_t fb_state_init(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_MOVEMENT_DETECTED:
            fb_transition(&fb_state_advertising);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fb_state_t fb_state_sleep(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //turn off all lights
            fb_set_led(FB_OFF);
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
        case FB_EVT_LOW_POWER:
            fb_transition(&fb_state_low_power);
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

fb_state_t fb_state_connected(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //set leds blue
            fb_set_led(FB_BLUE);
        break;
        case FB_EVT_EXIT:
            //turn leds off
            fb_set_led(FB_OFF);
        break;
        case FB_EVT_BLE_DISCONNECT:
            fb_transition(&fb_state_advertising);
        break;
        case FB_EVT_MOVEMENT_TIMEOUT:
            fb_transition(&fb_state_sleep);
        break;
        case FB_EVT_LOW_POWER:
            fb_transition(&fb_state_low_power);
        break;
        case FB_EVT_CHARGING_BEGIN:
            fb_transition(&fb_state_charging);
        break;
        case FB_EVT_REQUEST_KNIT:
            fb_transition(&fb_state_knit);
        break;
        case FB_EVT_REQUEST_PURL:
            fb_transition(&fb_state_purl);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}
fb_state_t fb_state_knit(const fb_event_t event){

    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //turn on blue led
            fb_set_led(FB_BLUE);
            //start knit animation
        break;
        case FB_EVT_EXIT:
            //turn off blue led
            fb_set_led(FB_OFF);
            //stop knit animation
        break;
        case FB_EVT_BLE_DISCONNECT:
            fb_transition(&fb_state_advertising);
        break;
        case FB_EVT_MOVEMENT_TIMEOUT:
            fb_transition(&fb_state_sleep);
        break;
        case FB_EVT_LOW_POWER:
            fb_transition(&fb_state_low_power);
        break;
        case FB_EVT_CHARGING_BEGIN:
            fb_transition(&fb_state_charging);
        break;
        case FB_EVT_KNIT_DETECTED:
            fb_transition(&fb_state_correct);
        break;
        case FB_EVT_PURL_DETECTED:
            fb_transition(&fb_state_incorrect);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fb_state_t fb_state_purl(const fb_event_t event){

    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //turn on blue led
            fb_set_led(FB_BLUE);
            //start purl animation
        break;
        case FB_EVT_EXIT:
            //turn off blue led
            fb_set_led(FB_OFF);
            //stop purl animation
        break;
        case FB_EVT_BLE_DISCONNECT:
            fb_transition(&fb_state_advertising);
        break;
        case FB_EVT_MOVEMENT_TIMEOUT:
            fb_transition(&fb_state_sleep);
        break;
        case FB_EVT_LOW_POWER:
            fb_transition(&fb_state_low_power);
        break;
        case FB_EVT_CHARGING_BEGIN:
            fb_transition(&fb_state_charging);
        break;
        case FB_EVT_KNIT_DETECTED:
            fb_transition(&fb_state_incorrect);
        break;
        case FB_EVT_PURL_DETECTED:
            fb_transition(&fb_state_correct);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fb_state_t fb_state_correct(const fb_event_t event){

    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //turn on green led
            fb_set_led(FB_GREEN);
            //turn on motor
        break;
        case FB_EVT_EXIT:
            //turn off green led
            fb_set_led(FB_OFF);
            //turn off motor
        break;
        case FB_EVT_BLE_DISCONNECT:
            fb_transition(&fb_state_advertising);
        break;
        case FB_EVT_MOVEMENT_TIMEOUT:
            fb_transition(&fb_state_sleep);
        break;
        case FB_EVT_LOW_POWER:
            fb_transition(&fb_state_low_power);
        break;
        case FB_EVT_CHARGING_BEGIN:
            fb_transition(&fb_state_charging);
        break;
        case FB_EVT_ANIMATION_END:
            fb_transition(&fb_state_connected);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}
fb_state_t fb_state_incorrect(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //turn on red led
            fb_set_led(FB_RED);
            //turn on motor
        break;
        case FB_EVT_EXIT:
            //turn off red led
            fb_set_led(FB_OFF);
            //turn off motor
        break;
        case FB_EVT_BLE_DISCONNECT:
            fb_transition(&fb_state_advertising);
        break;
        case FB_EVT_MOVEMENT_TIMEOUT:
            fb_transition(&fb_state_sleep);
        break;
        case FB_EVT_LOW_POWER:
            fb_transition(&fb_state_low_power);
        break;
        case FB_EVT_CHARGING_BEGIN:
            fb_transition(&fb_state_charging);
        break;
        case FB_EVT_ANIMATION_END:
            fb_transition(&fb_state_connected);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fb_state_t fb_state_low_power(const fb_event_t event){
   ret_code_t err_code = NRF_SUCCESS;
   switch(event){
        case FB_EVT_ENTRY:
            //turn yellow leds on
            fb_set_led(FB_YELLOW);
        break;
        case FB_EVT_EXIT:
            //turn yellow leds off
            fb_set_led(FB_OFF);
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


fb_state_t fb_state_charging(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //begin softblink
        break;
        case FB_EVT_EXIT:
            //stop softblink
        break;
        case FB_EVT_CHARGER_DISCONNECT:
            fb_transition(&fb_state_sleep);
        break;
        case FB_EVT_CHARGING_COMPLETE:
            fb_transition(&fb_state_charged);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}
fb_state_t fb_state_charged(const fb_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FB_EVT_ENTRY:
            //set led green
            fb_set_led(FB_GREEN);
        break;
        case FB_EVT_EXIT:
            //set led off
            fb_set_led(FB_OFF);
        break;
        case FB_EVT_CHARGER_DISCONNECT:
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

void fb_set_led(fb_led_mask_t colour_mask){
    pin_mask = FB_LED_PINS;
    NRF_GPIO->OUT = (NRF_GPIO->OUT &~pin_mask) | colour_mask;
}

