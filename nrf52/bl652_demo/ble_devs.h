#ifndef _BLE_DEV_SERV_H__
#define _BLE_DEV_SERV_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"


/**@brief   Macro for defining a ble_dev_serv instance.
 *
 *@param   _name   Name of the instance.
 *@hideinitializer
*/
#define BLE_DEVS_DEF(_name) 			\
static ble_devs_t _name;			\
NRF_SDH_BLE_OBSERVER(_name ## _obs,		\
		BLE_HRS_BLE_OBSERVER_PRIO,	\
		ble_devs_on_ble_evt, &_name)

// 93e6af3d-20b6-4a6d-9250-ce8d584b70df
#define DEV_SERVICE_UUID_BASE         {0x93, 0xE6, 0xAF, 0x3D, 0x20, 0xB6, 0x4A, 0x6D, \
                                              0x92, 0x50, 0xCE, 0x8D, 0x58, 0x4B, 0x70, 0xDF}
#define DEV_SERVICE_UUID    0x1400
#define QUAT_CHAR_UUID      0x1401
#define FEEDBACK_CHAR_UUID  0x1402

typedef enum{
    BLE_DEVS_EVT_NOTIFICATION_ENABLED,
    BLE_DEVS_EVT_NOTIFICATION_DISABLED,
	BLE_DEVS_EVT_DISCONNECTED,
	BLE_DEVS_EVT_CONNECTED
}ble_devs_evt_type_t;

/**@brief Custom Service event. */
typedef struct{
	ble_devs_evt_type_t evt_type;
}ble_devs_evt_t;

// Forward declaration of the ble_devs_t type.
typedef struct ble_devs_s ble_devs_t;

/**@brief Custom Service event handler type. */
typedef void (*ble_devs_evt_handler_t) (ble_devs_t * p_devs, ble_devs_evt_t * p_evt);


/**@brief Custom Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
	ble_devs_evt_handler_t		evt_handler;
        uint8_t                       	initial_dev_value;           /**< Initial custom value */
        ble_srv_cccd_security_mode_t  	value_char_attr_md;     /**< Initial security level for Custom characteristics attribute */
} ble_devs_init_t;


/**@brief Custom Service structure. This contains various status information for the service. */
struct ble_devs_s
{
	ble_devs_evt_handler_t		evt_handler;
        uint16_t                        service_handle;             /**< Handle of Custom Service (as provided by the BLE stack). */
        ble_gatts_char_handles_t        quat_handles;           /**< Handles related to the Custom Value characteristic. */
        ble_gatts_char_handles_t        feedback_handles;           /**< Handles related to the Custom Value characteristic. */
        uint16_t                        conn_handle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
        uint8_t                       uuid_type; 
};

/**@brief Function for initializing the Custom Service.
 *
 * @param[out]  p_devs       Custom Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_devs_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_devs_init(ble_devs_t * p_devs, const ble_devs_init_t * p_devs_init);


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @note 
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 * @param[in]   p_context  Custom Service structure.
 */
void ble_devs_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);

/**@brief Function for adding the Dev Quat characteristic.
 *
 * @param[in]   p_devs        Dev Service structure.
 * @param[in]   p_devs_init   Information needed to initialize the service.
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_devs_quat_char_add(ble_devs_t * p_devs, const ble_devs_init_t * p_devs_init);


/*@brief Function for updating the dev quat value.
 *
 * @detials The application calls this function when the quat value should be updated. If notification has been enable, the quat characteristic is sent to the client.
 *
 * @note
 *
 * @para[in]    p_devs          Dev Service structure.
 * @param[in]   Custom value
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_devs_quat_value_update(ble_devs_t * p_devs, uint8_t quat_value[8]);

/**@brief Function for adding the Dev feedback characteristic.
 *
 * @param[in]   p_devs        Dev Service structure.
 * @param[in]   p_devs_init   Information needed to initialize the service.
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_devs_feedback_char_add(ble_devs_t * p_devs, const ble_devs_init_t * p_devs_init);


/*@brief Function for updating the dev feedback value.
 *
 * @detials The application calls this function when the feedback value should be updated. If notification has been enable, the feedback characteristic is sent to the client.
 *
 * @note
 *
 * @para[in]    p_devs          Dev Service structure.
 * @param[in]   Custom value
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_devs_feedback_value_update(ble_devs_t * p_devs, uint8_t * feedback_value);

#endif
