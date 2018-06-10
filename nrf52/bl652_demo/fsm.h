#ifndef __FSM_H_
#define __FSM_H_


#include <stdint.h>

#define FSM_CMD_MD           0
#define FSM_CMD_AT           1
#define FSM_CMD_CD           2
#define FSM_CMD_LP           3
#define FSM_CMD_CB           4
#define FSM_CMD_CC           5
#define FSM_CMD_MT           6
#define FSM_CMD_BC           7
#define FSM_CMD_BD           8
#define FSM_CMD_RK           9
#define FSM_CMD_RP           10
#define FSM_CMD_KD           11
#define FSM_CMD_PD           12
#define FSM_CMD_AE           13

typedef enum{
    FSM_EVT_INIT,
    FSM_EVT_ENTRY,
    FSM_EVT_EXIT,
    FSM_EVT_MOVEMENT_TIMEOUT,
    FSM_EVT_ADVERTISING_TIMEOUT,
    FSM_EVT_CHARGER_DISCONNECT,
    FSM_EVT_LOW_POWER,
    FSM_EVT_CHARGING_BEGIN,
    FSM_EVT_CHARGING_COMPLETE,
    FSM_EVT_MOVEMENT_DETECTED,
    FSM_EVT_BLE_CONNECT,
    FSM_EVT_BLE_DISCONNECT,
    FSM_EVT_REQUEST_KNIT,
    FSM_EVT_REQUEST_PURL,
    FSM_EVT_KNIT_DETECTED,
    FSM_EVT_PURL_DETECTED,
    FSM_EVT_ANIMATION_END,
} fsm_event_t;

uint32_t (* fsm_handler)(const fsm_event_t event);

typedef uint32_t fsm_state_t;
void fsm_transition( fsm_state_t (*new_state)(const fsm_event_t event) );

fsm_state_t fsm_state_init(const fsm_event_t event);
fsm_state_t fsm_state_sleep(const fsm_event_t event);
fsm_state_t fsm_state_advertising(const fsm_event_t event);
fsm_state_t fsm_state_connected(const fsm_event_t event);
fsm_state_t fsm_state_knit(const fsm_event_t event);
fsm_state_t fsm_state_purl(const fsm_event_t event);
fsm_state_t fsm_state_correct(const fsm_event_t event);
fsm_state_t fsm_state_incorrect(const fsm_event_t event);
fsm_state_t fsm_state_low_power(const fsm_event_t event);
fsm_state_t fsm_state_charging(const fsm_event_t event);
fsm_state_t fsm_state_charged(const fsm_event_t event);


void fsm_init(void);


#endif
