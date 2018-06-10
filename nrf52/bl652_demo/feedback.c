#include "feedback.h"
#include "nrf_gpio.h"
#include "led_softblink.h"
#include "sdk_common.h"


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

}

void fb_set_led(fb_led_mask_t colour_mask){
    uint32_t pin_mask = FB_LED_PINS;
    NRF_GPIO->OUT = (NRF_GPIO->OUT &~pin_mask) | colour_mask;
}

void fb_start_softblink_purl(void){
    ret_code_t err_code;
    const led_sb_init_params_t led_sb_params = FB_REQ_PURL_SOFTBLINK_PARAMS;
    err_code = led_softblink_init(&led_sb_params);
    APP_ERROR_CHECK(err_code);
    err_code = led_softblink_start(FB_RED);
    APP_ERROR_CHECK(err_code);
}

void fb_start_softblink_knit(void){
    ret_code_t err_code;
    const led_sb_init_params_t led_sb_params = FB_REQ_KNIT_SOFTBLINK_PARAMS;
    err_code = led_softblink_init(&led_sb_params);
    APP_ERROR_CHECK(err_code);
    err_code = led_softblink_start(FB_GREEN);
    APP_ERROR_CHECK(err_code);
}
void fb_start_softblink_charging(void){
    ret_code_t err_code;
    const led_sb_init_params_t led_sb_params = FB_CHARGING_SOFTBLINK_PARAMS;
    err_code = led_softblink_init(&led_sb_params);
    APP_ERROR_CHECK(err_code);
    err_code = led_softblink_start(FB_WHITE);
    APP_ERROR_CHECK(err_code);
}
void fb_start_softblink_advertising(void){
    ret_code_t err_code;
    const led_sb_init_params_t led_sb_params = FB_ADVERTISING_SOFTBLINK_PARAMS;
    err_code = led_softblink_init(&led_sb_params);
    APP_ERROR_CHECK(err_code);
    err_code = led_softblink_start(FB_BLUE);
    APP_ERROR_CHECK(err_code);
}
void fb_stop_softblink(void){
    ret_code_t err_code;
    err_code = led_softblink_stop();
    APP_ERROR_CHECK(err_code);
    err_code = led_softblink_uninit();
}

