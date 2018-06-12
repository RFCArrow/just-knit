#include "interrupts.h"

#include <stdint.h>

#include "imu.h"
#include "fsm.h"
#include "nrf_drv_gpiote.h"
#include "nrf_gpio.h"

#define IMU_INT_PIN                 NRF_GPIO_PIN_MAP(0,29)
#define PGOOD_INT_PIN               NRF_GPIO_PIN_MAP(0,31)
#define CHG_INT_PIN                 NRF_GPIO_PIN_MAP(0,17)


void intr_init(){
    ret_code_t err_code;
    nrf_gpio_cfg_input(IMU_INT_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(PGOOD_INT_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(CHG_INT_PIN, NRF_GPIO_PIN_NOPULL);
    //Ensure GPIOTE is initialized
    if(!nrf_drv_gpiote_is_init()){
        err_code = nrf_drv_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }

    intr_set_pgood_interrupt();
    intr_set_chg_interrupt();

    //Set initial motion interrupt 
    nrf_drv_gpiote_in_config_t imu_int_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    err_code = nrf_drv_gpiote_in_init(IMU_INT_PIN, &imu_int_config, intr_handle_motion_interrupt);
    APP_ERROR_CHECK(err_code);

    imu_configure_interrupts();    

}

void intr_set_pgood_interrupt(){
    ret_code_t err_code;
    //Set rising interrupt
    nrf_drv_gpiote_in_config_t pgood_int_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    err_code = nrf_drv_gpiote_in_init(PGOOD_INT_PIN, &pgood_int_config, intr_handle_pgood_interrupt);
    APP_ERROR_CHECK(err_code);
}

void intr_handle_pgood_interrupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    fsm_handler(FSM_EVT_CHARGER_DISCONNECT);
}

void intr_set_chg_interrupt(){
    ret_code_t err_code;
    nrf_drv_gpiote_in_config_t chg_int_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    err_code = nrf_drv_gpiote_in_init(CHG_INT_PIN, &chg_int_config, intr_handle_chg_interrupt);
    APP_ERROR_CHECK(err_code);
}

void intr_handle_chg_interrupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    //If rising
    if(nrf_gpio_pin_read(CHG_INT_PIN) == 1){
        if(nrf_gpio_pin_read(PGOOD_INT_PIN) == 0){
            fsm_handler(FSM_EVT_CHARGING_COMPLETE);
        }
    }
    //If falling
    else{
        fsm_handler(FSM_EVT_CHARGING_BEGIN);
    }
}




//Set BNO055 to give rising edge on any motion
void intr_set_motion_interrupt(){
    //Delete existing interrupt on pin (for no motion)
    nrf_drv_gpiote_in_uninit(IMU_INT_PIN);

    imu_no_motion_interrupt(INTR_DISABLE);

    //Setup GPIOTE interrupt and attach callback
    nrf_drv_gpiote_in_config_t imu_int_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    ret_code_t err_code = nrf_drv_gpiote_in_init(IMU_INT_PIN, &imu_int_config, intr_handle_motion_interrupt);
    APP_ERROR_CHECK(err_code);

    imu_any_motion_interrupt(INTR_ENABLE);
}

void intr_handle_motion_interrupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    //reset pin low	
    bno055_set_intr_rst(INTR_ENABLE);
    //pass event to fsm
    fsm_handler(FSM_EVT_MOVEMENT_DETECTED);
}


//Set BNO055 to give rising edge after period of no motion
void intr_set_no_motion_interrupt(){
     //Delete existing interrupt on pin (for no motion)
    nrf_drv_gpiote_in_uninit(IMU_INT_PIN);

    imu_any_motion_interrupt(INTR_DISABLE);

    //Set new interrupt
    nrf_drv_gpiote_in_config_t imu_int_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    ret_code_t err_code = nrf_drv_gpiote_in_init(IMU_INT_PIN, &imu_int_config, intr_handle_no_motion_interrupt);
    APP_ERROR_CHECK(err_code);
    
    imu_no_motion_interrupt(INTR_ENABLE);
}

void intr_handle_no_motion_interrupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
	//reset pin low
    bno055_set_intr_rst(INTR_ENABLE);
    //pass event to fsm
    fsm_handler(FSM_EVT_MOVEMENT_TIMEOUT);
}
