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
#include "nvdm.h"

#ifndef MTK_DEBUG_LEVEL_NONE
log_create_module(rinlink_at, PRINT_LEVEL_INFO);
#define RINLINK_AT_LOG(x, ...)    LOG_I(rinlink_at, x, ##__VA_ARGS__)
#else
#define RINLINK_AT_LOG(x, ...)
#endif

extern int lsoc_char_mode;  // if set socket char mode, the user can send char ,not send hex 


apb_proxy_status_t apb_proxy_hdlr_socket_charmode_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	configASSERT(parse_cmd != NULL);
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
	char temp_buffer[50] = {0};
	nvdm_status_t status;
	char *p1 = NULL, *p2 = NULL;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    printf("%s", parse_cmd->string_ptr);
	
	 switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:  // rec: AT+LSOCCHARMODE=<op>...  the handler need to parse the parameters
		{
			p1 = strchr(parse_cmd->string_ptr,'=');
			if(p1 != NULL)
			{
				lsoc_char_mode = atoi(p1 + 1);
				response.result_code = APB_PROXY_RESULT_OK;			
			}else{
				response.result_code = APB_PROXY_RESULT_ERROR;
			}
			
			break;
		}
		case APB_PROXY_CMD_MODE_READ:   // rec: AT+LSOCCHARMODE?
        {
            sprintf(temp_buffer,"+LSOCCHARMODE:%d\r\n",lsoc_char_mode);
			response.pdata = temp_buffer;
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
		case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+LSOCCHARMODE=?
        {
			response.pdata = "+LSOCCHARMODE:<0,1>";
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
			
            break;
        }
		case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+LSOCCHARMODE
        {
            sprintf(temp_buffer,"+LSOCCHARMODE:%d\r\n",lsoc_char_mode);
			response.pdata = temp_buffer;
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
	 }
	 
	 response.cmd_id = parse_cmd->cmd_id;
	 apb_proxy_send_at_cmd_result(&response);
	 
    return APB_PROXY_STATUS_OK;
}


static int stopATLock_mode=0;
void connLayerStopSleepLockTimer(void);

apb_proxy_status_t apb_proxy_hdlr_StopATLock_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	configASSERT(parse_cmd != NULL);
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
	char temp_buffer[50] = {0};
	nvdm_status_t status;
	char *p1 = NULL, *p2 = NULL;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    printf("%s", parse_cmd->string_ptr);
	
	 switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:  // rec: AT+STOPATLOCK=<op>...  the handler need to parse the parameters
		{
			p1 = strchr(parse_cmd->string_ptr,'=');
			if(p1 != NULL)
			{
				connLayerStopSleepLockTimer();
				stopATLock_mode = atoi(p1 + 1);
				response.result_code = APB_PROXY_RESULT_OK;			
			}else{
				response.result_code = APB_PROXY_RESULT_ERROR;
			}
			
			break;
		}
		case APB_PROXY_CMD_MODE_READ:   // rec: AT+STOPATLOCK?
        {
            sprintf(temp_buffer,"+STOPATLOCK:%d\r\n",stopATLock_mode);
			response.pdata = temp_buffer;
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
		case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+STOPATLOCK=?
        {
			response.pdata = "+STOPATLOCK:<0,1>";
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
			
            break;
        }
		case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+STOPATLOCK
        {
			connLayerStopSleepLockTimer();
			stopATLock_mode = 1;
            sprintf(temp_buffer,"+STOPATLOCK:OK\r\n");
			response.pdata = temp_buffer;
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
	 }
	 
	 response.cmd_id = parse_cmd->cmd_id;
	 apb_proxy_send_at_cmd_result(&response);
	 
    return APB_PROXY_STATUS_OK;
}

uint32_t get_vbat_value(void)
{
	uint32_t adc_counts;
	uint32_t adc_voltage;
	hal_adc_init();     
	hal_adc_get_data_polling(HAL_ADC_CHANNEL_6, &adc_counts);
	adc_voltage = (adc_counts * 5600) / 4096;
	hal_adc_deinit();
	return adc_voltage;
}

apb_proxy_status_t apb_proxy_hdlr_get_vbat_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
	char temp[50]={0};
	uint32_t battery=0;
	char state=0;
	configASSERT(p_parse_cmd != NULL);
    apb_proxy_at_cmd_result_t cmd_result;
	
	battery = get_vbat_value();
	sprintf(temp,"VBAT:%dmV\r\n", battery);
	
    cmd_result.result_code = APB_PROXY_RESULT_OK;
    cmd_result.pdata = temp;
    cmd_result.length = strlen((const char*)(cmd_result.pdata));
    cmd_result.cmd_id = p_parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

apb_proxy_status_t apb_proxy_hdlr_sendcnt_cmd(apb_proxy_parse_cmd_param_t *parse_cmd)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	configASSERT(parse_cmd != NULL);
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
	char temp_buffer[50] = {0};
	nvdm_status_t status;
	char *p1 = NULL, *p2 = NULL;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    printf("%s", parse_cmd->string_ptr);
	
	 switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:  // rec: AT+SENDCNT=<op>...  the handler need to parse the parameters
		{
			p1 = strchr(parse_cmd->string_ptr,'=');
			if(p1 != NULL)
			{			
				unsigned int data;
				
				data = atoi(p1 + 1);
				SetSendCNT(data);
				response.result_code = APB_PROXY_RESULT_OK;			
			}else{
				response.result_code = APB_PROXY_RESULT_ERROR;
			}
			
			break;
		}
		case APB_PROXY_CMD_MODE_READ:   // rec: AT+SENDCNT?
		case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+SENDCNT
        {
            sprintf(temp_buffer,"+SENDCNT:%d\r\n",GetSendCNT());
			response.pdata = temp_buffer;
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
            break;
        }
		case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+SENDCNT=?
        {
			response.pdata = "+SENDCNT:<0,1,2,3...>";
			response.length = strlen((char *)response.pdata);
			response.result_code = APB_PROXY_RESULT_OK;
			
            break;
        }
	 }
	 
	 response.cmd_id = parse_cmd->cmd_id;
	 apb_proxy_send_at_cmd_result(&response);
	 
    return APB_PROXY_STATUS_OK;
}



uint32_t rin_read_defaultset_num(void)
{
	nvdm_status_t status;
	uint32_t blocknum = 55;
    uint32_t len = sizeof(blocknum);

    status = nvdm_read_data_item("rinset", "monday", (uint8_t *)&blocknum, &len);
	if(status != NVDM_STATUS_OK)
	{
		fprintf( stderr,"%s,line=%d  read block num fail status = %d\r\n",__FUNCTION__,__LINE__,status);
		blocknum = 0;
	}
	else
	{
		fprintf( stderr,"%s,line=%d  read block num= %d\r\n",__FUNCTION__,__LINE__,blocknum);
	}
	return blocknum;
}

int32_t rin_write_defaultset_num(int32_t num)
{
	nvdm_status_t status;
	uint32_t block = num;
	
    status = nvdm_write_data_item("rinset", "monday", NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t *)&block, sizeof(block));
	if(status != NVDM_STATUS_OK)
	{
		fprintf( stderr,"%s,line=%d  write block num fail status = %d\r\n",__FUNCTION__,__LINE__,status);
		return -1;
	}
	else
	{
		fprintf( stderr,"%s,line=%d  write block num= %d\r\n",__FUNCTION__,__LINE__,num);
		return 0;
	}
}




