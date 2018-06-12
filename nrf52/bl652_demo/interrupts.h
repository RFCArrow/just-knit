#ifndef __INTERRUPTS_H_
#define __INTERRUPTS_H_

#include "nrf_drv_gpiote.h"

#define INTR_DISABLE    0x00
#define INTR_ENABLE     0x01

void intr_init();
void intr_set_pgood_interrupt();
void intr_set_chg_interrupt();
void intr_set_motion_interrupt();
void intr_handle_pgood_interrupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void intr_handle_chg_interrupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void intr_handle_motion_interrupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void intr_set_no_motion_interrupt();
void intr_handle_no_motion_interrupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

#endif
