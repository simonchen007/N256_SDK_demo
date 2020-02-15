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
#include "FreeRTOSConfig.h"
#include "apb_proxy.h"
#include "syslog.h"
#include "td1030.h"
#include "nvdm.h"

#ifndef MTK_DEBUG_LEVEL_NONE
log_create_module(rinlink_example, PRINT_LEVEL_INFO);
#define RINLINK_EXAMPLE_LOG(x, ...)    LOG_I(rinlink_example, x, ##__VA_ARGS__)
#else
#define RINLINK_EXAMPLE_LOG(x, ...)
#endif
/*
* xufushuai 2018/04/03 begin
*/
apb_proxy_status_t apb_proxy_hdlr_device_name_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
	apb_proxy_at_cmd_result_t response;
	memset(&response, 0, sizeof(apb_proxy_at_cmd_result_t));
	response.pdata = "N301";
    response.length = strlen((char *)response.pdata);
    response.result_code = APB_PROXY_RESULT_OK;
	response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);
	
   return APB_PROXY_STATUS_OK;
}

/*xufushuai 2018/04/03 end*/
apb_proxy_status_t apb_proxy_hdlr_uart0_to_uart2_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
	configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;
	printf("\r\np_parse_cmd->string_ptr=%s\r\n",p_parse_cmd->string_ptr);
	
	char serial_buffer[512]={0};
	
    switch (p_parse_cmd->mode) {
        
        case APB_PROXY_CMD_MODE_EXECUTION: {         
			memcpy(serial_buffer, p_parse_cmd->string_ptr + p_parse_cmd->parse_pos , p_parse_cmd->string_len - p_parse_cmd->parse_pos);
			serial_buffer[p_parse_cmd->string_len - p_parse_cmd->parse_pos] = '\n';
			//serial_buffer[p_parse_cmd->string_len - p_parse_cmd->parse_pos +1] = '\n';
			hal_uart_send_dma(HAL_UART_2,serial_buffer,p_parse_cmd->string_len - p_parse_cmd->parse_pos +1);
			printf("\r\n----------------------- xufushuai ----------- %s, %s\r\n",__func__,serial_buffer);
        }
    }
    cmd_result.result_code = APB_PROXY_RESULT_CUSTOM_ERROR;
    cmd_result.pdata = "OK\r\n";
    cmd_result.length = strlen((const char*)(cmd_result.pdata));
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}
/*
* xufushuai 2018/04/10 begin
*/
extern TD1030_RMC rmc_data;

apb_proxy_status_t apb_proxy_hdlr_device_gps_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	configASSERT(parse_cmd != NULL);
    apb_proxy_at_cmd_result_t response = {0};
	char temp_buffer[50] = {0};
	int interval = 0;
	int sleep = 0;
	nvdm_status_t status;
	char *p1 = NULL, *p2 = NULL;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    printf("%s", parse_cmd->string_ptr);
	
	 switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:  // rec: AT+GPSGETLOCATION=<op>...  the handler need to parse the parameters
		{
			p1 = strchr(parse_cmd->string_ptr,'=');
			if(p1 != NULL)
			{
				interval = atoi(p1 + 1);
				p2 = strchr(p1 + 1,',');
				if(p2 != NULL)
				{
					sleep = atoi(p2 + 1);
				}
			
			
				if(p2 != NULL)
				{
					
					if(sleep == 0 || sleep == 1 || sleep == 2)
					{
						td1030_sleep_setting(sleep);
						/*
						memset(temp_buffer, 0, sizeof(temp_buffer));
						sprintf(temp_buffer,"%d",sleep);
						status = nvdm_write_data_item("RinlinkGps", "sleep", NVDM_DATA_ITEM_TYPE_STRING, temp_buffer, strlen((char *)temp_buffer));
						if (NVDM_STATUS_OK != status) {
						   printf("\r\nw fail RinlinkGps sleep \r\n");
						}else{
						   printf("\r\n w-------------------------- RinlinkGps sleep = %s\r\n",sleep);	
						}*/
					}
					if(interval == 100 || interval == 500 || interval == 1000 || interval == 10000 || interval == 60000)
					{
						receive_interval(interval);
						/*memset(temp_buffer, 0, sizeof(temp_buffer));
						sprintf(temp_buffer,"%d",interval);
						status = nvdm_write_data_item("RinlinkGps", "interval", NVDM_DATA_ITEM_TYPE_STRING, temp_buffer, strlen((char *)temp_buffer));
						if (NVDM_STATUS_OK != status) {
						   printf("\r\nw fail RinlinkGps interval \r\n");
						}else{
						   printf("\r\n w-------------------------- RinlinkGps interval = %s\r\n",interval);	
						}*/
					
					}
					response.result_code = APB_PROXY_RESULT_OK;
				}else{
					response.result_code = APB_PROXY_RESULT_ERROR;
				}
			}else{
				response.result_code = APB_PROXY_RESULT_ERROR;
			}
			
			break;
		}
		case APB_PROXY_CMD_MODE_READ:   // rec: AT+GPSGETLOCATION?
        {
            sprintf(temp_buffer,"+GPSGETLOCATION:%s,%c,%s,%c",rmc_data.Latitudinal,rmc_data.NorthSouth,rmc_data.Longitude,rmc_data.EastWest);
			response.pdata = temp_buffer;
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
		case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+GPSGETLOCATION=?
        {
			response.pdata = "+GPSGETLOCATION:<1,10,60,100,500>,<0,1,2>";
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
			
            break;
        }
		case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+GPSGETLOCATION
        {
            response.pdata = "+GPSGETLOCATION: not supported";
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
	 }
	 
	 response.cmd_id = parse_cmd->cmd_id;
	 apb_proxy_send_at_cmd_result(&response);
	 
    return APB_PROXY_STATUS_OK;
}
/*xufushuai 2018/04/10 end*/

extern TD1030_GSV gsv_dataGP;
extern TD1030_GSV gsv_dataBD;

extern int gpsdebug;

apb_proxy_status_t apb_proxy_hdlr_device_gps_state_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	configASSERT(parse_cmd != NULL);
    apb_proxy_at_cmd_result_t response = {0};
	char temp_buffer[50] = {0};
	int interval = 0;
	nvdm_status_t status;
	char *p1 = NULL, *p2 = NULL;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    printf("%s", parse_cmd->string_ptr);
	
	 switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:  // rec: AT+GPSSTATE=<op>...  the handler need to parse the parameters
		{		
			p1 = strchr(parse_cmd->string_ptr,'=');
			if(p1 != NULL)
			{
				gpsdebug = atoi(p1 + 1);
				sprintf(temp_buffer,"+GPSSTATE:gpsdebug=%d",gpsdebug);
				response.result_code = APB_PROXY_RESULT_OK;
			}else{
				response.result_code = APB_PROXY_RESULT_ERROR;
			}

			response.pdata = temp_buffer;
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
			break;
		}
		case APB_PROXY_CMD_MODE_READ:   // rec: AT+GPSSTATE?
        {
            sprintf(temp_buffer,"+GPSSTATE:GP=%d,BD=%d,GPCN=%d,BDCN=%d",gsv_dataGP.TotalSatellite,gsv_dataBD.TotalSatellite,gsv_dataGP.CN,gsv_dataBD.CN);
			response.pdata = temp_buffer;
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
		case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+GPSSTATE=?
        {
			response.pdata = "+GPSSTATE:<NUM>,<CN>";
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
			
            break;
        }
		case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+GPSSTATE
        {
            response.pdata = "+GPSSTATE: not supported";
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
	 }
	 
	 response.cmd_id = parse_cmd->cmd_id;
	 apb_proxy_send_at_cmd_result(&response);
	 
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_device_gpstest_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
	configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;
	printf("\r\np_parse_cmd->string_ptr=%s\r\n",p_parse_cmd->string_ptr);
	
	gps_rest();
	
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = "OK\r\n";
    cmd_result.length = strlen((const char*)(cmd_result.pdata));
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_device_gpscheckcn_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
	char temp_buffer[50] = {0};
    char cn = 0;
	
	configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;

	cn = (gsv_dataGP.CN > gsv_dataBD.CN ? gsv_dataGP.CN : gsv_dataBD.CN);
	
	sprintf(temp_buffer,"+GPSCHKCN:%d\r\n",cn);
	
	if(cn > 10){
		cmd_result.result_code = APB_PROXY_RESULT_OK;
	}else{
		cmd_result.result_code = APB_PROXY_RESULT_ERROR;
	}
	
    cmd_result.pdata = temp_buffer;
    cmd_result.length = strlen((const char*)(cmd_result.pdata));
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}


