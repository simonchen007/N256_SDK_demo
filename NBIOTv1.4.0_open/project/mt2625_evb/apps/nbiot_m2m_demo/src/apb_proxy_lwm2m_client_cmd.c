/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#include "stdio.h"
#include "string.h"
#include "apb_proxy.h"
#include "syslog.h"

#ifdef LLLWM2M_DEMO_SUPPORT
#include "LLTypeDef.h"
#endif
#ifdef MTK_LWM2M_CT_SUPPORT
#include "liblwm2m.h"
#include "apb_proxy_nw_cmd_gprot.h"
#include "app_common_header.h"

#include "hal_rtc_external.h"
#include "nvdm.h"
#include "ril.h"

#define LWM2M_CLI_LOG(fmt, args...)    LOG_I(lwm2m_cli, "[LWM2M_CLI]"fmt, ##args)
log_create_module(lwm2m_cli, PRINT_LEVEL_INFO);

#define CTIOT_LWM2M_CLIENT_NVDM_GROUP_NAME          "ct_lwm2m"
#define CTIOT_LWM2M_CLIENT_NVDM_ITEM_NAME_CONFIG    "config"
#define CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN           128

extern nb_app_context g_nb_app_context;
extern void lwm2m_client_init();
extern void lwm2m_client_quit(char *title, int quit);
extern void lwm2m_client_deregister(void);
extern bool lwm2m_client_is_ready(void);

#ifdef LLLWM2M_DEMO_SUPPORT
lwm2m_creat g_parse_cmd = {0};
#endif

static void ctiot_lwm2m_client_send_response(char *pdata)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.pdata = pdata;
    response.length = strlen(pdata);
    apb_proxy_send_at_cmd_result(&response);
}

void ctiot_lwm2m_client_observe_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:observe success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_notify_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:notify success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_register_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:register success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_register_failed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:register failed";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_register_update_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:register update success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
}

void ctiot_lwm2m_client_deregister_success(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *response_data = "+M2MCLI:deregister success";

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ctiot_lwm2m_client_send_response(response_data);
    if (g_nb_app_context.wait_for_deleted == true) {
        lwm2m_client_quit("LWM2M_CLI", 3);
        g_nb_app_context.wait_for_deleted = false;
    }
}

void ctiot_lwm2m_client_receive_data(uint8_t *data, uint32_t data_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char *prefix = "+M2MCLIRECV:";
    char *response_data;
    int i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    LWM2M_CLI_LOG("data = 0x%x, data_len = %d", data, data_len);
    for (i = 0; i < data_len; i++) {
        LWM2M_CLI_LOG("0x%x", data[i]);
    }
    // +M2MCLIRECV:<Data>
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
    response_data = (char *)pvPortMalloc(strlen(prefix) + 2 * data_len + 1);
#else
    response_data = (char *)pvPortMalloc(strlen(prefix) + data_len + 1);
#endif
    if (response_data == NULL) {
        return;
    }

    strcpy(response_data, prefix);
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
    onenet_at_bin_to_hex(response_data + strlen(prefix), data, 2 * data_len);
#else
    if (data_len > 3) {
        /* 1 byte message_id, 2 bytes length */
        memcpy(response_data + strlen(prefix), data + 3, data_len - 3);
        response_data[strlen(prefix) + data_len - 3] = '\0';
    }
#endif
    ctiot_lwm2m_client_send_response(response_data);
    vPortFree(response_data);
}

static int32_t ctiot_lwm2m_client_ril_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
    switch (event_id) {
        case RIL_URC_ID_MATWAKEUP:
            LWM2M_CLI_LOG("receive RIL wakeup urc");
            if (lwm2m_client_is_ready() == true) {
                uint32_t len;
                char *cmd_string = (char *)pvPortMalloc(CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN);
                char *param_buffer = (char *)pvPortMalloc(CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN);
                len = CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN;
                nvdm_read_data_item(CTIOT_LWM2M_CLIENT_NVDM_GROUP_NAME,
                                    CTIOT_LWM2M_CLIENT_NVDM_ITEM_NAME_CONFIG,
                                    (uint8_t *)cmd_string,
                                    &len);
                char *param_list[6];
                uint32_t param_num = onenet_at_parse_cmd(cmd_string, param_buffer, param_list, 6);
                char *server = param_list[0];
                char *port = param_list[1];
                char *name = param_list[2];
                int lifetime = atoi(param_list[3]);
                g_nb_app_context.server = (char *)pvPortMalloc(strlen(server) + 1);
                strcpy(g_nb_app_context.server, server);
                g_nb_app_context.port = (char *)pvPortMalloc(strlen(port) + 1);
                strcpy(g_nb_app_context.port, port);
                g_nb_app_context.name = (char *)pvPortMalloc(strlen(name) + 1);
                strcpy(g_nb_app_context.name, name);
#ifdef WITH_TINYDTLS
                if (param_num > 5) {
                    char *pskid = param_list[4];
                    char *psk = param_list[5];
                    g_nb_app_context.pskid = (char *)pvPortMalloc(strlen(pskid) + 1);
                    strcpy(g_nb_app_context.pskid, pskid);
                    g_nb_app_context.psk = (char *)pvPortMalloc(strlen(psk) + 1);
                    strcpy(g_nb_app_context.psk, psk);
                }
#endif
                g_nb_app_context.lifetime = lifetime;
                g_nb_app_context.activated_from_command = true;
                lwm2m_client_init();
                vPortFree(param_buffer);
                vPortFree(cmd_string);
            }
            break;

        default:
            break;
    }

    return 0;
}

void ctiot_lwm2m_client_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP) {
        /* COLD-BOOT case: normal init */
    } else {
        /* DEEP-SLEEP case: data retention process */
        LWM2M_CLI_LOG("deep sleep handling");
        ril_register_event_callback(RIL_GROUP_MASK_ALL, ctiot_lwm2m_client_ril_urc_callback);
    }
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_create_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    LWM2M_CLI_LOG("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                LWM2M_CLI_LOG("memory error");
                response.result_code = APB_PROXY_RESULT_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    LWM2M_CLI_LOG("no parameter");
                    response.result_code = APB_PROXY_RESULT_ERROR;
                } else {
                    char *param_list[6];
                    nvdm_write_data_item(CTIOT_LWM2M_CLIENT_NVDM_GROUP_NAME,
                                 CTIOT_LWM2M_CLIENT_NVDM_ITEM_NAME_CONFIG,
                                 NVDM_DATA_ITEM_TYPE_STRING,
                                 (const uint8_t *)(cmd_string + 1),
                                 CTIOT_LWM2M_CLIENT_MAX_CONFIG_LEN);
                    LWM2M_CLI_LOG("cmd_string=%s", cmd_string);
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, 6);

                    // AT+M2MCLINEW=<server>,<port>,<name>,<lifetime>[,<pskid>,<psk>]
                    if (param_num < 4) {
                        LWM2M_CLI_LOG("parameter too short");
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else if (g_nb_app_context.lwm2m_info != NULL) {
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else {
                        char *server = param_list[0];
                        char *port = param_list[1];
                        char *name = param_list[2];
                        int lifetime = atoi(param_list[3]);
                        g_nb_app_context.server = (char *)pvPortMalloc(strlen(server) + 1);
                        strcpy(g_nb_app_context.server, server);
                        g_nb_app_context.port = (char *)pvPortMalloc(strlen(port) + 1);
                        strcpy(g_nb_app_context.port, port);
                        g_nb_app_context.name = (char *)pvPortMalloc(strlen(name) + 1);
                        strcpy(g_nb_app_context.name, name);
#ifdef WITH_TINYDTLS
                        if (param_num > 5) {
                            char *pskid = param_list[4];
                            char *psk = param_list[5];
                            g_nb_app_context.pskid = (char *)pvPortMalloc(strlen(pskid) + 1);
                            strcpy(g_nb_app_context.pskid, pskid);
                            g_nb_app_context.psk = (char *)pvPortMalloc(strlen(psk) + 1);
                            strcpy(g_nb_app_context.psk, psk);
                        }
#endif
                        g_nb_app_context.lifetime = lifetime;
                        g_nb_app_context.activated_from_command = true;
                        lwm2m_client_init();
                        response.result_code = APB_PROXY_RESULT_OK;
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            LWM2M_CLI_LOG("not support");
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_delete_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    LWM2M_CLI_LOG("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_ACTIVE:
            if (g_nb_app_context.lwm2m_info == NULL) {
                response.result_code = APB_PROXY_RESULT_ERROR;
            } else {
                // AT+M2MCLIDEL
                if (g_nb_app_context.server != NULL) {
                    vPortFree(g_nb_app_context.server);
                    g_nb_app_context.server = NULL;
                }
                if (g_nb_app_context.port != NULL) {
                    vPortFree(g_nb_app_context.port);
                    g_nb_app_context.port = NULL;
                }
                if (g_nb_app_context.name != NULL) {
                    vPortFree(g_nb_app_context.name);
                    g_nb_app_context.name = NULL;
                }
                if (g_nb_app_context.pskid != NULL) {
                    vPortFree(g_nb_app_context.pskid);
                    g_nb_app_context.pskid = NULL;
                }
                if (g_nb_app_context.psk != NULL) {
                    vPortFree(g_nb_app_context.psk);
                    g_nb_app_context.psk = NULL;
                }
                g_nb_app_context.lifetime = 0;
                g_nb_app_context.activated_from_command = false;
                g_nb_app_context.wait_for_deleted = true;
                lwm2m_client_deregister();
                response.result_code = APB_PROXY_RESULT_OK;
            }
            break;

        default:
            LWM2M_CLI_LOG("not support");
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_send_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
    bool need_send_quickly = false;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    LWM2M_CLI_LOG("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                LWM2M_CLI_LOG("memory error");
                response.result_code = APB_PROXY_RESULT_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    LWM2M_CLI_LOG("no parameter");
                    response.result_code = APB_PROXY_RESULT_ERROR;
                } else {
                    char *param_list[1];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, 1);

                    // AT+M2MCLISEND=<Data>
                    if (param_num < 1) {
                        LWM2M_CLI_LOG("parameter too short");
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else if (strlen(param_list[0]) % 2 != 0) {
                        LWM2M_CLI_LOG("length of <Data> is odd");
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else {
                        char *obj_uri_string = "/19/0/0";
                        lwm2m_uri_t uri;
                        uint8_t *config_bin;
                        uint32_t config_len = strlen(param_list[0]) / 2;
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
                        config_bin = (uint8_t *)pvPortMalloc(config_len + 1);
                        config_len = onenet_at_hex_to_bin(config_bin, param_list[0], config_len);
#else
                        /* 1 byte message_id, 2 bytes length */
                        config_bin = (uint8_t *)pvPortMalloc(config_len + 4);
                        config_bin[0] = 0;
                        if (config_len <= 0xFF) {
                            config_bin[1] = 0;
                            config_bin[2] = config_len;
                        } else {
                            config_bin[1] = (config_len & 0xFF00) >> 8;
                            config_bin[2] = (config_len & 0x00FF);
                        }
                        config_len = onenet_at_hex_to_bin(config_bin + 3, param_list[0], config_len);
                        config_len += 3;
                        LWM2M_CLI_LOG("config_bin[1-2] = 0x%x 0x%x", config_bin[1], config_bin[2]);
#endif
                        lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);
                        if (g_nb_app_context.lwm2m_info == NULL) {
                            response.result_code = APB_PROXY_RESULT_ERROR;
                        } else {
                            handle_value_changed(g_nb_app_context.lwm2m_info, &uri, (const char *)config_bin, config_len);
                            response.result_code = APB_PROXY_RESULT_OK;
                            need_send_quickly = true;
                        }
                        vPortFree(config_bin);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            LWM2M_CLI_LOG("not support");
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    if (need_send_quickly == true) {
        extern void lwm2m_step_quickly(lwm2m_context_t * contextP);
        lwm2m_step_quickly(g_nb_app_context.lwm2m_info);
    }

    return APB_PROXY_STATUS_OK;
}

#ifdef LLLWM2M_DEMO_SUPPORT
void apb_proxy_hdlr_lwm2m_client_create(lwm2m_creat parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    printf("\r\nip=%s, port=%s, deviceid=%s, lifetime=%s, pskid=%s, psk=%s\r\n", parse_cmd.ip,parse_cmd.port,parse_cmd.deviceID,parse_cmd.lifeTime,parse_cmd.pskID,parse_cmd.psk);
    memcpy(&g_parse_cmd, &parse_cmd, sizeof(parse_cmd));
	
	g_nb_app_context.server = (char *)pvPortMalloc(strlen(parse_cmd.ip) + 1);
	strcpy(g_nb_app_context.server, parse_cmd.ip);
	g_nb_app_context.port = (char *)pvPortMalloc(strlen(parse_cmd.port) + 1);
	strcpy(g_nb_app_context.port, parse_cmd.port);
	g_nb_app_context.name = (char *)pvPortMalloc(strlen(parse_cmd.deviceID) + 1);
	strcpy(g_nb_app_context.name, parse_cmd.deviceID);
	if (parse_cmd.pskID[0] != NULL) {
		g_nb_app_context.pskid = (char *)pvPortMalloc(strlen(parse_cmd.pskID) + 1);
		strcpy(g_nb_app_context.pskid, parse_cmd.pskID);
		g_nb_app_context.psk = (char *)pvPortMalloc(strlen(parse_cmd.psk) + 1);
		strcpy(g_nb_app_context.psk, parse_cmd.psk);
	}
	g_nb_app_context.lifetime = atoi(parse_cmd.lifeTime);
	g_nb_app_context.activated_from_command = true;
	lwm2m_client_init();
                       
}

void apb_proxy_hdlr_lwm2m_client_send(char *string_ptr, uint32_t string_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
   // char *param_buffer;
	bool need_send_quickly = false;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   // printf("\r\n ---------------------------   -------------------%s\r\n", string_ptr);
	{
		
		{
			char *obj_uri_string = "/19/0/0";
			lwm2m_uri_t uri;
			uint8_t *config_bin;
			uint32_t config_len = string_len;//strlen(param_list[0]) / 2;
#ifdef USE_HAIWEI_RAWDATA_PLUGIN
			config_bin = (uint8_t *)pvPortMalloc(config_len + 1);
			memcpy(config_bin, string_ptr, config_len);
			//config_len = onenet_at_hex_to_bin(config_bin, param_list[0], config_len);
#else
#endif
			lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);
			if (g_nb_app_context.lwm2m_info == NULL) {
				response.result_code = APB_PROXY_RESULT_ERROR;
			} else {
				handle_value_changed(g_nb_app_context.lwm2m_info, &uri, (const char *)config_bin, config_len);
				response.result_code = APB_PROXY_RESULT_OK;
				need_send_quickly = true;
			}
			vPortFree(config_bin);
		}
	}

	//if (param_buffer != NULL) {
	//	vPortFree(param_buffer);
	//}
	// if (need_send_quickly == true) {
    //    extern void lwm2m_step_quickly(lwm2m_context_t * contextP);
    //    lwm2m_step_quickly(g_nb_app_context.lwm2m_info);
   // }
}

void apb_proxy_hdlr_lwm2m_client_delete()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   if (g_nb_app_context.lwm2m_info == NULL) {
               // response.result_code = APB_PROXY_RESULT_ERROR;
   } else {
		if (g_nb_app_context.server != NULL) {
			vPortFree(g_nb_app_context.server);
			g_nb_app_context.server = NULL;
		}
		if (g_nb_app_context.port != NULL) {
			vPortFree(g_nb_app_context.port);
			g_nb_app_context.port = NULL;
		}
		if (g_nb_app_context.name != NULL) {
			vPortFree(g_nb_app_context.name);
			g_nb_app_context.name = NULL;
		}
		if (g_nb_app_context.pskid != NULL) {
			vPortFree(g_nb_app_context.pskid);
			g_nb_app_context.pskid = NULL;
		}
		if (g_nb_app_context.psk != NULL) {
			vPortFree(g_nb_app_context.psk);
			g_nb_app_context.psk = NULL;
		}
		g_nb_app_context.lifetime = 0;
		g_nb_app_context.activated_from_command = false;
		g_nb_app_context.wait_for_deleted = true;
	    lwm2m_client_deregister();
	}
	
}
#endif

#ifdef ADUPS_FOTA_ENABLE_CT
void abup_send_reg_info(char *data,uint8_t len)
{
	char *param_buffer;
	param_buffer = (char *)pvPortMalloc(len);

	if (param_buffer == NULL) 
	{
		LWM2M_CLI_LOG("memory error");
    } 
    else 
    {
        char *cmd_string = strchr(data, '=');
        char *param_list[1];
        uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, 1);
        char *obj_uri_string = "/19/0/0";
        lwm2m_uri_t uri;
        uint8_t *config_bin;
        uint32_t config_len = strlen(param_list[0]) / 2;

        /* 1 byte message_id, 2 bytes length */
        config_bin = (uint8_t *)pvPortMalloc(config_len + 1);
		if(config_bin)
		{
	        config_len = onenet_at_hex_to_bin(config_bin, param_list[0], config_len);

	        lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);
	        if (g_nb_app_context.lwm2m_info != NULL) 
	        {
				LWM2M_CLI_LOG("abup send reg msg\r\n");
	            handle_value_changed(g_nb_app_context.lwm2m_info, &uri, (const char *)config_bin, config_len);
	        }
	        vPortFree(config_bin);
        }
    }
    if (param_buffer != NULL) 
    {
        vPortFree(param_buffer);
    }
            
}

apb_proxy_status_t abup_apb_proxy_hdlr_lwm2m_client_send_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    LWM2M_CLI_LOG("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                LWM2M_CLI_LOG("memory error");
                response.result_code = APB_PROXY_RESULT_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    LWM2M_CLI_LOG("no parameter");
                    response.result_code = APB_PROXY_RESULT_ERROR;
                } else {
                    char *param_list[1];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, 1);

                    // AT+M2MCLISEND=<Data>
                    if (param_num < 1) {
                        LWM2M_CLI_LOG("parameter too short");
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else if (strlen(param_list[0]) % 2 != 0) {
                        LWM2M_CLI_LOG("length of <Data> is odd");
                        response.result_code = APB_PROXY_RESULT_ERROR;
                    } else {
                        char *obj_uri_string = "/19/0/0";
                        lwm2m_uri_t uri;
                        uint8_t *config_bin;
                        uint32_t config_len = strlen(param_list[0]) / 2;

                        /* 1 byte message_id, 2 bytes length */
                        config_bin = (uint8_t *)pvPortMalloc(config_len + 1);

                        config_len = onenet_at_hex_to_bin(config_bin, param_list[0], config_len);
                        //config_len += 3;
                        //LWM2M_CLI_LOG("config_bin[1-2] = 0x%x 0x%x", config_bin[1], config_bin[2]);

                        lwm2m_stringToUri(obj_uri_string, strlen(obj_uri_string), &uri);
                        if (g_nb_app_context.lwm2m_info == NULL) {
                            response.result_code = APB_PROXY_RESULT_ERROR;
                        } else {
                            handle_value_changed(g_nb_app_context.lwm2m_info, &uri, (const char *)config_bin, config_len);
                            response.result_code = APB_PROXY_RESULT_OK;
                        }
                        vPortFree(config_bin);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }
            break;

        default:
            LWM2M_CLI_LOG("not support");
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}
#endif


#else /* MTK_LWM2M_CT_SUPPORT */

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_create_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_delete_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_lwm2m_client_send_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    response.result_code = APB_PROXY_RESULT_ERROR;
    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}

#endif /* MTK_LWM2M_CT_SUPPORT */

