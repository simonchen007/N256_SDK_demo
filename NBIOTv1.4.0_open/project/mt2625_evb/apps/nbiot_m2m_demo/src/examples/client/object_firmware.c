/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Julien Vermillard - initial implementation
 *    Fabien Fleutot - Please refer to git log
 *    David Navarro, Intel Corporation - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/

/*
 * This object is single instance only, and provide firmware upgrade functionality.
 * Object ID is 5.
 */

/*
 * resources:
 * 0 package                   write
 * 1 package url               write
 * 2 update                    exec
 * 3 state                     read
 * 4 update supported objects  read/write
 * 5 update result             read
 */

#include "liblwm2m.h"
#include "app_common_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "syslog.h"
#include "fota.h"
#include "FreeRTOS.h"

log_create_module(obj_firmware, PRINT_LEVEL_INFO);

/******************************************************************************************
 *                               Local Macro Definition                                   *
 *****************************************************************************************/
#define OBJ_FIRMWARE_ERR(fmt,arg...)   LOG_E(obj_firmware, "[Obj_firmware]: "fmt,##arg)
#define OBJ_FIRMWARE_WARN(fmt,arg...)  LOG_W(obj_firmware, "[Obj_firmware]: "fmt,##arg)
#define OBJ_FIRMWARE_DBG(fmt,arg...)   LOG_I(obj_firmware,"[Obj_firmware]: "fmt,##arg)


// ---- private object "Firmware" specific defines ----
// Resource Id's:
#define RES_M_PACKAGE                   0
#define RES_M_PACKAGE_URI               1
#define RES_M_UPDATE                    2
#define RES_M_STATE                     3
#define RES_O_UPDATE_SUPPORTED_OBJECTS  4
#define RES_M_UPDATE_RESULT             5
#define RES_O_PKG_NAME                  6
#define RES_O_PKG_VERSION               7

const char *obj_state_uri_string = "/5/0/3";
const char *obj_update_result_uri_string = "/5/0/5";
extern nb_app_context g_nb_app_context;

typedef enum {
    OBJECT_FIRMWARE_STATE_IDLE               = 0,
    OBJECT_FIRMWARE_STATE_DOWNLOADING        = 1,
    OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED = 2,
    OBJECT_FIRMWARE_STATE_UPGRADING          = 3,
    OBJECT_FIRMWARE_STATE_UPGRADE_COMPLETED  = 4
}object_firmware_state_t;

typedef struct
{
    uint8_t state;
    bool supported;
    uint8_t result;
} firmware_data_t;

typedef struct
{
    object_firmware_state_t firmware_state;
    fota_result_t result;
    uint32_t progress_info;
} firmware_cur_state_t;


static firmware_cur_state_t g_firmware_cur_state = {0};

#ifdef ADUPS_FOTA_ENABLE_CT

#define ABUP_MANUFACTURER      		"any"   			//OEM
#define ABUP_MODEL_NUMBER      		"chenjing"			//…Ë±∏–Õ∫≈ device id
#define ABUP_PLATFORM 				"MT8321"
#define ABUP_DEVICE_TYPE 			"bicycle"

#define ABUP_SDK_VERSION 			"IOT4.0_R40232"
#define ABUP_APK_VERSION 			"ADUPS_V4.0"
#define ABUP_FIRMWARE_VERSION_PRE   "0.0"
#define ABUP_FIRMWARE_VERSION  		"1.0"

#define ABUP_NETWORK_TYPE			"NB"

#define ABUP_PRODUCT_ID 			"1517694829"
//#define ABUP_PRODUCT_SEC 			"87c008da13bc4ab09132c4585bd37e10"





#include "hal_wdt.h"
static void object_firmware_send_data(char* obj_uri, uint32_t data);

static uint8_t reg_data[255];
static uint8_t reg_len = 0;
static uint8_t abup_current_op = 0;
static uint8_t abup_mid[20];

void abup_add_reg_param(uint8_t *data, uint8_t len)
{
	uint8_t i = 0;
	if(reg_len == 0)
	{
		sprintf(reg_data+reg_len,"%s",data);
		reg_len += len;
	}
	else
	{
		sprintf(reg_data+reg_len,"%02x",len);
		reg_len += 2;
		for(i = 0; i < len; i++)
		{
			sprintf(reg_data+reg_len,"%02x",data[i]);
			reg_len += 2;
		}
	}
	
}
/*
01 reg
03 rep download result
04 rep update result
*/

int32_t abup_do_reg_query_imsi_callback(ril_cmd_response_t *rsp)
{
	long utc_time = 0;
	uint32_t result = 0;
    uint8_t templen = 0;
    uint8_t tempstr[50] = {0};
    ril_imsi_rsp_t *param = (ril_imsi_rsp_t*)rsp->cmd_param;
    
    if (rsp->res_code == RIL_RESULT_CODE_OK && param!= NULL){
        
        templen = strlen(param->imsi);
        fprintf(stderr,"param->imsi = %s,%d\r\n", param->imsi, templen);
        
        memset(tempstr,0,sizeof(tempstr));
        memset(abup_mid,0,sizeof(abup_mid));
        
        memcpy(tempstr, param->imsi, templen);
        memcpy(abup_mid, param->imsi, templen);
      
    } else {
        fprintf(stderr,"Query imsi result fail!\r\n");
        memset(tempstr,0,sizeof(tempstr));
        memset(abup_mid,0,sizeof(abup_mid));
        strcpy(tempstr, "abupdevmid");
        strcpy(abup_mid, "abupdevmid");
        templen = strlen("abupdevmid");
    }

    reg_len = 0;
	memset(reg_data, 0, sizeof(reg_data));
	abup_add_reg_param("AT+ABUPSEND=01", strlen("AT+ABUPSEND=01"));
	abup_add_reg_param(ABUP_PRODUCT_ID, strlen(ABUP_PRODUCT_ID));
	abup_add_reg_param(tempstr,templen); //mid
	abup_add_reg_param(ABUP_MANUFACTURER, strlen(ABUP_MANUFACTURER));
	abup_add_reg_param(ABUP_MODEL_NUMBER, strlen(ABUP_MODEL_NUMBER));
	
	
	abup_add_reg_param(ABUP_PLATFORM, strlen(ABUP_PLATFORM));
	abup_add_reg_param(ABUP_DEVICE_TYPE, strlen(ABUP_DEVICE_TYPE));
	abup_add_reg_param(ABUP_SDK_VERSION, strlen(ABUP_SDK_VERSION));
	abup_add_reg_param(ABUP_APK_VERSION, strlen(ABUP_APK_VERSION));
	abup_add_reg_param(ABUP_FIRMWARE_VERSION, strlen(ABUP_FIRMWARE_VERSION));
	abup_add_reg_param(ABUP_FIRMWARE_VERSION_PRE, strlen(ABUP_FIRMWARE_VERSION_PRE));
	abup_add_reg_param(ABUP_NETWORK_TYPE, strlen(ABUP_NETWORK_TYPE));
	utc_time = lwm2m_gettime();
	memset(tempstr,0,sizeof(tempstr));
	sprintf(tempstr, "%d", utc_time);
	OBJ_FIRMWARE_DBG("abup_send_reg utc= %s\r\n",tempstr );
	abup_add_reg_param(tempstr, strlen(tempstr));
	
	
	output_buffer(stderr, reg_data, reg_len, 0);
	abup_send_reg_info(reg_data,reg_len);

	result = Adups_get_update_result();
	if(Adups_get_update_result() == 0 || Adups_get_update_result() == 1)
	{
		abup_report_update_result_timer();
	}

	
	return 0;
}

void abup_send_update_result(int8_t result)
{
	uint8_t tempstr[50] = {0};

	reg_len = 0;
	memset(reg_data, 0, sizeof(reg_data));

	abup_add_reg_param("AT+ABUPSEND=04", strlen("AT+ABUPSEND=04"));
	abup_add_reg_param(ABUP_PRODUCT_ID, strlen(ABUP_PRODUCT_ID));
	abup_add_reg_param(abup_mid,strlen(abup_mid)); //mid
	abup_add_reg_param(ABUP_FIRMWARE_VERSION, strlen(ABUP_FIRMWARE_VERSION));
	abup_add_reg_param(ABUP_FIRMWARE_VERSION_PRE, strlen(ABUP_FIRMWARE_VERSION_PRE));
	if(result == 0) //succeed
	{
		abup_add_reg_param("Y", 1);
	}
	else
	{
		abup_add_reg_param("N", 1);
	}
	output_buffer(stderr, reg_data, reg_len, 0);
	abup_send_reg_info(reg_data,reg_len);
}

void abup_send_download_result(int8_t result)
{
	uint8_t tempstr[50] = {0};

	reg_len = 0;
	memset(reg_data, 0, sizeof(reg_data));

	abup_add_reg_param("AT+ABUPSEND=03", strlen("AT+ABUPSEND=03"));
	abup_add_reg_param(ABUP_PRODUCT_ID, strlen(ABUP_PRODUCT_ID));
	abup_add_reg_param(abup_mid,strlen(abup_mid)); //mid
	abup_add_reg_param(ABUP_FIRMWARE_VERSION, strlen(ABUP_FIRMWARE_VERSION));
	abup_add_reg_param(ABUP_FIRMWARE_VERSION_PRE, strlen(ABUP_FIRMWARE_VERSION_PRE));
	if(result == 0) //succeed
	{
		abup_add_reg_param("Y", 1);
	}
	else
	{
		abup_add_reg_param("N", 1);
	}
	output_buffer(stderr, reg_data, reg_len, 0);
	abup_send_reg_info(reg_data,reg_len);
}
void adups_system_reboot()
{
	hal_wdt_config_t wdt_config;
	
    wdt_config.mode = HAL_WDT_MODE_RESET;
    wdt_config.seconds = 1;
    hal_wdt_init(&wdt_config);
    hal_wdt_software_reset();	
}

typedef enum {
    ABUP_IDLE			 = 0,
    ABUP_DOWNLOADING	 = 1,
	ABUP_PAUSE			 = 2,
}abup_state_t;


static volatile uint8_t abup_process = 0;
static void *firmware_data = NULL;
void abup_send_download_result_ct(uint8_t fota_result)
{
	//fota_result_t
	firmware_data_t *data = (firmware_data_t *)firmware_data;
	if(data)
	{
		data->state = OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED;
		OBJ_FIRMWARE_DBG("abup_send_download_result LIEN=%d\r\n", __LINE__);
	}
    g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED;
    
    OBJ_FIRMWARE_DBG("abup_send_download_result %d\r\n", fota_result);
    object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
}

void abup_notify_result(uint32_t val)
{
	fprintf("%s,line=%d,result=%d",__FUNCTION__,__LINE__,val);
	abup_process = ABUP_IDLE;
	switch(val)
	{
		case 0: //success
			abup_send_download_result_ct(0);
			abup_send_download_result(0);
			//waitting for execute
		break;
		case 1: //pause
		break;
		defualt: //failed
			abup_send_download_result(1);
		break;
	}	
}


void abup_report_update_result_timer_cb()
{
	// 0 success; 1 failed; 2 else
	uint8_t fota_result = Adups_get_update_result();
	if(fota_result != 2)
	{
		Adups_clear_update_result();
		abup_send_update_result(fota_result);
	}
}
	
void abup_set_process(uint8_t val)
{
	abup_process = val;
}

void abup_set_process_pause()
{
	abup_process = ABUP_PAUSE;
}

void abup_continue_download()
{
	abup_process = ABUP_DOWNLOADING;
	abup_download_image(NULL,0,1);
}
uint8_t abup_get_process()
{
	return abup_process;
}

#endif
static void object_firmware_send_data(char* obj_uri, uint32_t data)
{
    lwm2m_uri_t uri;
    lwm2m_stringToUri(obj_uri, strlen(obj_uri), &uri);
    char temp[32] = {0};
    OBJ_FIRMWARE_DBG("object firmware broadcast data\r\n");
    if (g_nb_app_context.lwm2m_info == NULL) {
        return;
    } else {
        snprintf(temp, sizeof(temp), "%d", data);
        handle_value_changed(g_nb_app_context.lwm2m_info, &uri, (const char *)temp, strlen(temp));
        OBJ_FIRMWARE_DBG("object firmware broadcast data OK\r\n");
    }
}

static void prv_firmware_fota_event_indication(fota_msg_event_t event, fota_msg_event_info_t* p_info)
{
    if (p_info == NULL)
    {
        return;
    }
    switch (event)
    {
        case FOTA_MSG_DOWNLOAD_RESULT_IND:
        {
            g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED;
            g_firmware_cur_state.result = p_info->fota_result;
            OBJ_FIRMWARE_DBG("FOTA_MSG_DOWNLOAD_RESULT_IND");
            object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
            break;
        }
        case FOTA_MSG_UPGRADE_RESULT_IND:
        {
            g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_UPGRADE_COMPLETED;
            g_firmware_cur_state.result = p_info->fota_result;
            OBJ_FIRMWARE_DBG("FOTA_MSG_UPGRADE_RESULT_IND");
            object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
            object_firmware_send_data(obj_update_result_uri_string, g_firmware_cur_state.result);
            break;
        }
        case FOTA_MSG_DOWNLOAD_PROGRESS_IND:
        {
            g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_DOWNLOADING;
            g_firmware_cur_state.progress_info = p_info->progress;
            object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
            break;
        }
        case FOTA_MSG_UPGRADE_PROGRESS_IND:
        {
            g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_UPGRADING;
            g_firmware_cur_state.progress_info = p_info->progress;
            object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
            break;
        }
        default:
        {
            break;
        }
    }
}

static uint8_t prv_firmware_read(uint16_t instanceId,
                                 int * numDataP,
                                 lwm2m_data_t ** dataArrayP,
                                 lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);
    data->state = g_firmware_cur_state.firmware_state;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        *dataArrayP = lwm2m_data_new(3);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = 3;
        (*dataArrayP)[0].id = 3;
        (*dataArrayP)[1].id = 4;
        (*dataArrayP)[2].id = 5;
    }

    i = 0;
    do
    {
        switch ((*dataArrayP)[i].id)
        {
        case RES_M_PACKAGE:
        case RES_M_PACKAGE_URI:
        case RES_M_UPDATE:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;

        case RES_M_STATE:
            // firmware update state (int)
            lwm2m_data_encode_int(data->state, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_O_UPDATE_SUPPORTED_OBJECTS:
            lwm2m_data_encode_bool(data->supported, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_M_UPDATE_RESULT: {
            OBJ_FIRMWARE_DBG("read upgrade result.");
            if (g_firmware_cur_state.firmware_state == OBJECT_FIRMWARE_STATE_IDLE)
            {
            #ifdef ADUPS_FOTA_ENABLE_CT
            	fota_result_t fota_result = Adups_get_update_result();
            #else
                fota_result_t fota_result = fota_read_upgrade_result();
            #endif    
                data->result = (uint8_t)fota_result;
                lwm2m_data_encode_int(data->result, *dataArrayP + i);
                result = COAP_205_CONTENT;
            }
            else if (g_firmware_cur_state.firmware_state == OBJECT_FIRMWARE_STATE_UPGRADE_COMPLETED)
            {
                data->result = g_firmware_cur_state.result;
                lwm2m_data_encode_int(data->result, *dataArrayP + i);
                result = COAP_205_CONTENT;
            }
            else
            {
                data->result = 0xFF;
                lwm2m_data_encode_int(data->result, *dataArrayP + i);
                result = COAP_205_CONTENT;
            }
            break;
        }
        default:
            result = COAP_404_NOT_FOUND;
        }

        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}


static uint8_t prv_firmware_write(uint16_t instanceId,
                                  int numData,
                                  lwm2m_data_t * dataArray,
                                  lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);
    uint32_t uri_len = 0;
    char* p_uri = NULL;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    i = 0;

    do
    {
        OBJ_FIRMWARE_DBG("dataArray[%d].type: %d, ID: %d\r\n", i, dataArray[i].type, dataArray[i].id);
        switch (dataArray[i].id)
        {
        case RES_M_PACKAGE:
            // inline firmware binary
            result = COAP_204_CHANGED;
            break;
        case RES_M_STATE:
            result = COAP_204_CHANGED;
            break;
        case RES_M_PACKAGE_URI:
            // URL for download the firmware
            uri_len = dataArray[i].value.asBuffer.length;
            p_uri = pvPortMalloc(uri_len + 1);
            if (p_uri != NULL){
                fota_result_t fota_result;
                memset(p_uri, 0, uri_len + 1);
                memcpy(p_uri, dataArray[i].value.asBuffer.buffer, uri_len);
                OBJ_FIRMWARE_DBG("the firmware download URI:%s\r\n", p_uri);
				#ifdef ADUPS_FOTA_ENABLE_CT
				if(abup_process == ABUP_IDLE)
				{
					abup_process = ABUP_DOWNLOADING;
					hal_uart_send_dma(HAL_UART_0, p_uri, uri_len);
					
					fota_result = abup_download_image(p_uri,uri_len,0);
				}
				#else
                fota_result = fota_download_image(FOTA_ALL_BIN, p_uri);
                #endif
                if (fota_result == FOTA_OK){
                    g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_DOWNLOADING;
                    data->state = g_firmware_cur_state.firmware_state;
                    data->result = fota_result;
                    object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
                }else {
                    g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED;
                    data->state = g_firmware_cur_state.firmware_state;
                    data->result = fota_result;
                    object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
                }
                vPortFree(p_uri);
                p_uri = NULL;
            }
            result = COAP_204_CHANGED;
            break;

        case RES_O_UPDATE_SUPPORTED_OBJECTS:
            if (lwm2m_data_decode_bool(&dataArray[i], &data->supported) == 1)
            {
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
        }

        i++;
    } while (i < numData && result == COAP_204_CHANGED);

    return result;
}

static uint8_t prv_firmware_execute(uint16_t instanceId,
                                    uint16_t resourceId,
                                    uint8_t * buffer,
                                    int length,
                                    lwm2m_object_t * objectP)
{
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);
    uint8_t* p_data = NULL;
    coap_status_t result = 0;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    if (length != 0) return COAP_400_BAD_REQUEST;

    // for execute callback, resId is always set.
    switch (resourceId)
    {
    case RES_M_UPDATE:{
        fota_result_t fota_result;
        OBJ_FIRMWARE_DBG("trigger fota update.");
#ifdef ADUPS_FOTA_ENABLE_CT
		if (g_firmware_cur_state.firmware_state == OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED){
			abup_prepare_reboot(10000); // 10s
		    //adups_system_reboot();
			result = COAP_204_CHANGED;
		}
		else
		{
			result = COAP_405_METHOD_NOT_ALLOWED;
		}
#else
        if ((g_firmware_cur_state.firmware_state == OBJECT_FIRMWARE_STATE_DOWNLOAD_COMPLETED)
            || (g_firmware_cur_state.firmware_state == OBJECT_FIRMWARE_STATE_IDLE)){
            fota_result = fota_trigger_upgrade(FOTA_ALL_BIN);
            if (fota_result == FOTA_OK){
                g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_UPGRADING;
                g_firmware_cur_state.result = fota_result;
                object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
            }else{
                g_firmware_cur_state.firmware_state = OBJECT_FIRMWARE_STATE_UPGRADE_COMPLETED;
                g_firmware_cur_state.result = fota_result;
                object_firmware_send_data(obj_state_uri_string, g_firmware_cur_state.firmware_state);
            }
            OBJ_FIRMWARE_DBG("trigger fota update,result:%d\r\n.", fota_result);
            result = COAP_204_CHANGED;
        }else{
            result = COAP_405_METHOD_NOT_ALLOWED;
        }
#endif        
        break;
    }
    default:
        result = COAP_405_METHOD_NOT_ALLOWED;
    }

    return result;
}

void display_firmware_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    firmware_data_t * data = (firmware_data_t *)object->userData;
    fprintf(stdout, "  /%u: Firmware object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    state: %u, supported: %s, result: %u\r\n",
                data->state, data->supported?"true":"false", data->result);
    }
#endif
}

lwm2m_object_t * get_object_firmware(void)
{
    /*
     * The get_object_firmware function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * firmwareObj;

    firmwareObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != firmwareObj)
    {
        memset(firmwareObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns its unique ID
         * The 5 is the standard ID for the optional object "Object firmware".
         */
        firmwareObj->objID = LWM2M_FIRMWARE_UPDATE_OBJECT_ID;

        /*
         * and its unique instance
         *
         */
        firmwareObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != firmwareObj->instanceList)
        {
            memset(firmwareObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(firmwareObj);
            return NULL;
        }

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        firmwareObj->readFunc    = prv_firmware_read;
        firmwareObj->writeFunc   = prv_firmware_write;
        firmwareObj->executeFunc = prv_firmware_execute;
        firmwareObj->userData    = lwm2m_malloc(sizeof(firmware_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != firmwareObj->userData)
        {
            ((firmware_data_t*)firmwareObj->userData)->state = 0;
            ((firmware_data_t*)firmwareObj->userData)->supported = false;
            ((firmware_data_t*)firmwareObj->userData)->result = 0;
            fota_register_event(prv_firmware_fota_event_indication); 
            #ifdef ADUPS_FOTA_ENABLE_CT
            firmware_data = (void *)firmwareObj->userData;
            abup_init();
            #endif
        }
        else
        {
            lwm2m_free(firmwareObj);
            firmwareObj = NULL;
        }
    }

    return firmwareObj;
}

void free_object_firmware(lwm2m_object_t * objectP)
{
    if (NULL != objectP->userData)
    {
        lwm2m_free(objectP->userData);
        objectP->userData = NULL;
    }
    if (NULL != objectP->instanceList)
    {
        lwm2m_free(objectP->instanceList);
        objectP->instanceList = NULL;
    }
    lwm2m_free(objectP);
}

