#include "fsm.h"
#include "feedback.h"
#include "sdk_common.h"

void fsm_init(void){
    fsm_handler = fsm_state_sleep;
}

void fsm_transition( fsm_state_t (*new_state)(const fsm_event_t event) ){
    fsm_handler(FSM_EVT_EXIT);
    fsm_handler = new_state;
    fsm_handler(FSM_EVT_ENTRY);
}

//Initial State
fsm_state_t fsm_state_init(const fsm_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_MOVEMENT_DETECTED:
            fsm_transition(&fsm_state_advertising);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fsm_state_t fsm_state_sleep(const fsm_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            //turn off all lights
            fb_set_led(FB_OFF);
            //set motion interrupt
        break;
        case FSM_EVT_EXIT:
            //set no-motion interrupt
        break;
        case FSM_EVT_MOVEMENT_DETECTED:
            fsm_transition(&fsm_state_advertising);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        default:
            //no need to do anything
        break;
    }
    return err_code;
}

fsm_state_t fsm_state_advertising(const fsm_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            fb_start_softblink_advertising();
        break;
        case FSM_EVT_EXIT:
            fb_stop_softblink();
        break;
        case FSM_EVT_BLE_CONNECT:
            fsm_transition(&fsm_state_connected);
        break;
        case FSM_EVT_ADVERTISING_TIMEOUT:
            fsm_transition(&fsm_state_sleep);
        break;
        case FSM_EVT_LOW_POWER:
            fsm_transition(&fsm_state_low_power);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fsm_state_t fsm_state_connected(const fsm_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            //set leds blue
            fb_set_led(FB_BLUE);
        break;
        case FSM_EVT_EXIT:
            //turn leds off
            fb_set_led(FB_OFF);
        break;
        case FSM_EVT_BLE_DISCONNECT:
            fsm_transition(&fsm_state_advertising);
        break;
        case FSM_EVT_MOVEMENT_TIMEOUT:
            fsm_transition(&fsm_state_sleep);
        break;
        case FSM_EVT_LOW_POWER:
            fsm_transition(&fsm_state_low_power);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        case FSM_EVT_REQUEST_KNIT:
            fsm_transition(&fsm_state_knit);
        break;
        case FSM_EVT_REQUEST_PURL:
            fsm_transition(&fsm_state_purl);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}
fsm_state_t fsm_state_knit(const fsm_event_t event){

    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            //set base colour to blue to match ready state
            fb_set_led(FB_BLUE);
            fb_start_softblink_knit();
        break;
        case FSM_EVT_EXIT:
            //turn off base colour
            fb_set_led(FB_OFF);
            fb_stop_softblink();
        break;
        case FSM_EVT_BLE_DISCONNECT:
            fsm_transition(&fsm_state_advertising);
        break;
        case FSM_EVT_MOVEMENT_TIMEOUT:
            fsm_transition(&fsm_state_sleep);
        break;
        case FSM_EVT_LOW_POWER:
            fsm_transition(&fsm_state_low_power);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        case FSM_EVT_KNIT_DETECTED:
            fsm_transition(&fsm_state_correct);
        break;
        case FSM_EVT_PURL_DETECTED:
            fsm_transition(&fsm_state_incorrect);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fsm_state_t fsm_state_purl(const fsm_event_t event){

    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            //set base colour to blue to match ready state
            fb_set_led(FB_BLUE);
            fb_start_softblink_purl();
        break;
        case FSM_EVT_EXIT:
            //turn off base colour
            fb_set_led(FB_OFF);
            fb_stop_softblink(); 
        break;
        case FSM_EVT_BLE_DISCONNECT:
            fsm_transition(&fsm_state_advertising);
        break;
        case FSM_EVT_MOVEMENT_TIMEOUT:
            fsm_transition(&fsm_state_sleep);
        break;
        case FSM_EVT_LOW_POWER:
            fsm_transition(&fsm_state_low_power);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        case FSM_EVT_KNIT_DETECTED:
            fsm_transition(&fsm_state_incorrect);
        break;
        case FSM_EVT_PURL_DETECTED:
            fsm_transition(&fsm_state_correct);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fsm_state_t fsm_state_correct(const fsm_event_t event){

    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            fb_set_led(FB_GREEN);
            //turn on motor
        break;
        case FSM_EVT_EXIT:
            fb_set_led(FB_OFF);
            //turn off motor
        break;
        case FSM_EVT_BLE_DISCONNECT:
            fsm_transition(&fsm_state_advertising);
        break;
        case FSM_EVT_MOVEMENT_TIMEOUT:
            fsm_transition(&fsm_state_sleep);
        break;
        case FSM_EVT_LOW_POWER:
            fsm_transition(&fsm_state_low_power);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        case FSM_EVT_ANIMATION_END:
            fsm_transition(&fsm_state_connected);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}
fsm_state_t fsm_state_incorrect(const fsm_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            fb_set_led(FB_RED);
            //turn on motor
        break;
        case FSM_EVT_EXIT:
            fb_set_led(FB_OFF);
            //turn off motor
        break;
        case FSM_EVT_BLE_DISCONNECT:
            fsm_transition(&fsm_state_advertising);
        break;
        case FSM_EVT_MOVEMENT_TIMEOUT:
            fsm_transition(&fsm_state_sleep);
        break;
        case FSM_EVT_LOW_POWER:
            fsm_transition(&fsm_state_low_power);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        case FSM_EVT_ANIMATION_END:
            fsm_transition(&fsm_state_connected);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}

fsm_state_t fsm_state_low_power(const fsm_event_t event){
   ret_code_t err_code = NRF_SUCCESS;
   switch(event){
        case FSM_EVT_ENTRY:
            fb_set_led(FB_YELLOW);
        break;
        case FSM_EVT_EXIT:
            fb_set_led(FB_OFF);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}


fsm_state_t fsm_state_charging(const fsm_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            fb_start_softblink_charging();
        break;
        case FSM_EVT_EXIT:
            fb_stop_softblink();
        break;
        case FSM_EVT_CHARGER_DISCONNECT:
            fsm_transition(&fsm_state_sleep);
        break;
        case FSM_EVT_CHARGING_COMPLETE:
            fsm_transition(&fsm_state_charged);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}
fsm_state_t fsm_state_charged(const fsm_event_t event){
    ret_code_t err_code = NRF_SUCCESS;
    switch(event){
        case FSM_EVT_ENTRY:
            fb_set_led(FB_GREEN);
        break;
        case FSM_EVT_EXIT:
            fb_set_led(FB_OFF);
        break;
        case FSM_EVT_CHARGER_DISCONNECT:
            fsm_transition(&fsm_state_sleep);
        break;
        case FSM_EVT_CHARGING_BEGIN:
            fsm_transition(&fsm_state_charging);
        break;
        default:
            //do nothing
        break;
    }
    return err_code;
}
