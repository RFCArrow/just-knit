#include "sdk_common.h"
#include "ble_srv_common.h"
#include "ble_devs.h"
#include <string.h>
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_log.h"

static void on_connect(ble_devs_t * p_devs, ble_evt_t const * p_ble_evt);
static void on_disconnect(ble_devs_t * p_devs, ble_evt_t const * p_ble_evt);
static void on_write(ble_devs_t * p_devs, ble_evt_t const * p_ble_evt);

uint32_t ble_devs_init(ble_devs_t * p_devs, const ble_devs_init_t * p_devs_init)
{
    if (p_devs == NULL || p_devs_init == NULL){
            return NRF_ERROR_NULL;
        }

    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_devs->evt_handler			= p_devs_init->evt_handler;
    p_devs->conn_handle               	= BLE_CONN_HANDLE_INVALID;


    // Add Custom Service UUID
    ble_uuid128_t base_uuid = {DEV_SERVICE_UUID_BASE};
    err_code =  sd_ble_uuid_vs_add(&base_uuid, &p_devs->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_devs->uuid_type;
    ble_uuid.uuid = DEV_SERVICE_UUID;

    // Add the Custom Service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_devs->service_handle);
    if (err_code != NRF_SUCCESS)
    {
            return err_code;
    }


    // Add Custom Value Characteristic
    err_code =  ble_devs_quat_char_add(p_devs, p_devs_init);
    VERIFY_SUCCESS(err_code);
    err_code = ble_devs_feedback_char_add(p_devs, p_devs_init);
    return err_code;
}

/**@brief Function for adding the Custom Value characteristic.
 *
 * @param[in]   p_devs        Custom Service structure.
 * @param[in]   p_devs_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_devs_quat_char_add(ble_devs_t * p_devs, const ble_devs_init_t * p_devs_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    // Read operation on cccd should be possible without authentication
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc        = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 1; 
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md; 
    char_md.p_sccd_md         = NULL;

    static char user_desc[] = "Quaternion";
    char_md.p_char_user_desc = (uint8_t *) user_desc;
    char_md.char_user_desc_size = strlen(user_desc);
    char_md.char_user_desc_max_size = strlen(user_desc);
    
    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_devs_init->value_char_attr_md.read_perm;
    attr_md.write_perm = p_devs_init->value_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = p_devs->uuid_type;
    ble_uuid.uuid = QUAT_CHAR_UUID;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid	= &ble_uuid;
    attr_char_value.p_attr_md	= &attr_md;
    attr_char_value.init_len	= 8*sizeof(uint8_t);
    attr_char_value.init_offs 	= 0;
    attr_char_value.max_len	= 8*sizeof(uint8_t);

    err_code = sd_ble_gatts_characteristic_add(p_devs->service_handle, &char_md, &attr_char_value, &p_devs->quat_handles);
    if(err_code != NRF_SUCCESS){
	return err_code;
    }
    return NRF_SUCCESS;

}

void ble_devs_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context){
    ble_devs_t * p_devs = (ble_devs_t *) p_context;
    
    if(p_devs == NULL || p_ble_evt == NULL){
	return;
    }

    switch (p_ble_evt->header.evt_id){
	case BLE_GAP_EVT_CONNECTED:
	    on_connect(p_devs, p_ble_evt);
	    break;

	case BLE_GAP_EVT_DISCONNECTED:
	    on_disconnect(p_devs, p_ble_evt);
	    break;

	case BLE_GATTS_EVT_WRITE:
	    on_write(p_devs, p_ble_evt);
	    break;

	default:
	     //Nop
	     break;
    }
}

/**@brief Function for handling the Connect event.
 *
 * @param[in]	p_devs		Dev Service Structure.
 * @param[in] 	p_ble_evt 	Event received rom the BLE stack.
 */

static void on_connect(ble_devs_t * p_devs, ble_evt_t const * p_ble_evt){
    p_devs->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

    ble_devs_evt_t evt;

    evt.evt_type = BLE_DEVS_EVT_CONNECTED;

    p_devs->evt_handler(p_devs, &evt);
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]	p_devs		Dev Service Structure.
 * @param[in] 	p_ble_evt 	Event received rom the BLE stack.
 */

static void on_disconnect(ble_devs_t * p_devs, ble_evt_t const * p_ble_evt){
    UNUSED_PARAMETER(p_ble_evt);
    p_devs->conn_handle = BLE_CONN_HANDLE_INVALID;
}

static void on_write(ble_devs_t * p_devs, ble_evt_t const * p_ble_evt){
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    //Check if the handle passed with the event matches the Dev Value Characteristic handle
    if(p_evt_write->handle == p_devs->feedback_handles.value_handle){
	//Put specific task here.
	nrf_gpio_pin_toggle(LED_4);
    }

    // Check if the Custom Valud CCCCD is written to and that the vlaue is the appropriate length, i.e 2 bytes.
    if ((p_evt_write->handle == p_devs->quat_handles.cccd_handle)
        && (p_evt_write-> len == 2)
    ){
        if(p_devs->evt_handler != NULL){
            ble_devs_evt_t evt;

            if(ble_srv_is_notification_enabled(p_evt_write->data)){
                evt.evt_type = BLE_DEVS_EVT_NOTIFICATION_ENABLED;
            }
            else{
                evt.evt_type = BLE_DEVS_EVT_NOTIFICATION_DISABLED;
            }
            p_devs->evt_handler(p_devs, &evt);
        }
    }
}

uint32_t ble_devs_quat_value_update(ble_devs_t * p_devs, uint8_t quat_value[8]){
    if(p_devs == NULL){
        return NRF_ERROR_NULL;
    }

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    //Initialise value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = 8;
    gatts_value.offset  = 0;
    gatts_value.p_value = quat_value;

    //Update database
    err_code = sd_ble_gatts_value_set(p_devs->conn_handle,
                                        p_devs->quat_handles.value_handle,
                                        &gatts_value);
    if(err_code != NRF_SUCCESS){
        return err_code;
    }

    // Send value if connected and notifying.
    if((p_devs->conn_handle != BLE_CONN_HANDLE_INVALID)){
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle   = p_devs->quat_handles.value_handle;
        hvx_params.type     = BLE_GATT_HVX_NOTIFICATION;    
        hvx_params.offset   = gatts_value.offset;
        hvx_params.p_len    = &gatts_value.len;
        hvx_params.p_data   = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(p_devs->conn_handle, &hvx_params);
    }
    else{
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}


uint32_t ble_devs_feedback_char_add(ble_devs_t * p_devs, const ble_devs_init_t * p_devs_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    // Read operation on cccd should be possible without authentication
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc        = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 1; 
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md; 
    char_md.p_sccd_md         = NULL;

    static char user_desc[] = "Feedback";
    char_md.p_char_user_desc = (uint8_t *) user_desc;
    char_md.char_user_desc_size = strlen(user_desc);
    char_md.char_user_desc_max_size = strlen(user_desc);

    
    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_devs_init->value_char_attr_md.read_perm;
    attr_md.write_perm = p_devs_init->value_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = p_devs->uuid_type;
    ble_uuid.uuid = FEEDBACK_CHAR_UUID;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid	= &ble_uuid;
    attr_char_value.p_attr_md	= &attr_md;
    attr_char_value.init_len	= sizeof(uint8_t);
    attr_char_value.init_offs 	= 0;
    attr_char_value.max_len	= sizeof(uint8_t);

    err_code = sd_ble_gatts_characteristic_add(p_devs->service_handle, &char_md, &attr_char_value, &p_devs->feedback_handles);
    if(err_code != NRF_SUCCESS){
	return err_code;
    }
    return NRF_SUCCESS;

}


uint32_t ble_devs_feedback_value_update(ble_devs_t * p_devs, uint8_t * feedback_value){
    if(p_devs == NULL){
        return NRF_ERROR_NULL;
    }

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    //Initialise value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = 1;
    gatts_value.offset  = 0;
    gatts_value.p_value = feedback_value;

    //Update database
    err_code = sd_ble_gatts_value_set(p_devs->conn_handle,
                                        p_devs->feedback_handles.value_handle,
                                        &gatts_value);
    if(err_code != NRF_SUCCESS){
        return err_code;
    }

    // Send value if connected and notifying.
    if((p_devs->conn_handle != BLE_CONN_HANDLE_INVALID)){
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle   = p_devs->feedback_handles.value_handle;
        hvx_params.type     = BLE_GATT_HVX_NOTIFICATION;    
        hvx_params.offset   = gatts_value.offset;
        hvx_params.p_len    = &gatts_value.len;
        hvx_params.p_data   = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(p_devs->conn_handle, &hvx_params);
    }
    else{
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}


