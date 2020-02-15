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
#ifdef MTK_ONENET_SUPPORT
#include "cis_def.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_def.h"
#endif
#include "apb_proxy.h"
#include "syslog.h"

#ifndef MTK_DEBUG_LEVEL_NONE
log_create_module(onenet_example, PRINT_LEVEL_INFO);
#define ONENET_EXAMPLE_LOG(x, ...)    LOG_I(onenet_example, x, ##__VA_ARGS__)
#else
#define ONENET_EXAMPLE_LOG(x, ...)
#endif

#ifdef MTK_ONENET_SUPPORT
#define APB_PROXY_ONENET_EXAMPLE_ENABLE
#endif


#ifdef APB_PROXY_ONENET_EXAMPLE_ENABLE

TaskHandle_t apb_proxy_onenet_example_task_hdl = NULL;


void apb_proxy_onenet_example_send_urc(int op, int result)
{
    char result_str[25] = {0};
    apb_proxy_at_cmd_result_t response = {0};

    ONENET_EXAMPLE_LOG(result_str, "+ONENETCLI:%d,%d", op, result);

    response.cmd_id = APB_PROXY_INVALID_CMD_ID;
    response.pdata = result_str;
    response.result_code = APB_PROXY_RESULT_UNSOLICITED;
    response.length = strlen((char *)response.pdata);
    apb_proxy_send_at_cmd_result(&response);
}


void apb_proxy_onenet_example_task_delete(void)
{
    if (!apb_proxy_onenet_example_task_hdl)
    {
        ONENET_EXAMPLE_LOG("[onenet] Task does not exist.");
        return;
    }

    apb_proxy_onenet_example_task_hdl = NULL;
    ONENET_EXAMPLE_LOG("[onenet] Task is deleted.");
    /* Delete current task. */
    vTaskDelete(NULL);    
}


static void apb_proxy_onenet_example_task_main(void *param)
{
    cis_asyn_main();

    apb_proxy_onenet_example_send_urc(0, 1);
    apb_proxy_onenet_example_task_delete();
}


int apb_proxy_onenet_example_task_create(void)
{
    if (apb_proxy_onenet_example_task_hdl)
    {
        ONENET_EXAMPLE_LOG("[onenet] Task has been created.");
        return -1;
    }

    cis_asyn_set_shutdown(false);

    xTaskCreate(apb_proxy_onenet_example_task_main,
                APB_PROXY_ONENET_EXAMPLE_TASK_NAME,
                APB_PROXY_ONENET_EXAMPLE_STACKSIZE / sizeof(portSTACK_TYPE),
                NULL,
                APB_PROXY_ONENET_EXAMPLE_PRIO,
                &apb_proxy_onenet_example_task_hdl);
    
    ONENET_EXAMPLE_LOG("[onenet] task hdl:%x.", apb_proxy_onenet_example_task_hdl);
    return apb_proxy_onenet_example_task_hdl ? 0 : -2;
}


int apb_proxy_onenet_example_shutdown(void)
{
    if (apb_proxy_onenet_example_task_hdl && !cis_asyn_get_shutdown())
    {
        cis_asyn_set_shutdown(true);
        return 0;
    }
    else
    {
        ONENET_EXAMPLE_LOG("[onenet] No need to shutdown.");
    }

    return -1;
}
#endif


apb_proxy_status_t apb_proxy_hdlr_onenet_client_cmd(apb_proxy_parse_cmd_param_t *p_parse_cmd)
{
    apb_proxy_at_cmd_result_t cmd_result = {0};

    ONENET_EXAMPLE_LOG("%s", __FUNCTION__);
    configASSERT(p_parse_cmd != NULL);

    cmd_result.result_code = APB_PROXY_RESULT_ERROR;
#ifdef APB_PROXY_ONENET_EXAMPLE_ENABLE
    switch (p_parse_cmd->mode)
    {
        case APB_PROXY_CMD_MODE_READ:   // rec: AT+ONENETCLI?
        {
            ONENET_EXAMPLE_LOG("Read mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_TESTING:    // rec: AT+ONENETCLI=?
        {
            ONENET_EXAMPLE_LOG("TEST mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_ACTIVE: // rec: AT+ONENETCLI
        {
            ONENET_EXAMPLE_LOG("ACTIVE mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_INVALID:
        {
            ONENET_EXAMPLE_LOG("INVALID mode is not supported.\n");
            break;
        }

        case APB_PROXY_CMD_MODE_EXECUTION: // rec: AT+ONENETCLI=<op>...  the handler need to parse the parameters
        {
            int ret = -1;
            char *param = NULL;

            param = p_parse_cmd->string_ptr + p_parse_cmd->name_len + 1;
            ONENET_EXAMPLE_LOG("param to parse: %s\r\n", param);
            if ('0' != param[0] && '1' != param[0])
            {
                ONENET_EXAMPLE_LOG("Invalid param.\n");
                break;
            }
            
            if ('1' == param[0])
            {
                ret = apb_proxy_onenet_example_task_create();
                if (0 == ret)
                {
                    cmd_result.result_code = APB_PROXY_RESULT_OK;
                }
            }
            else
            {
                ret = apb_proxy_onenet_example_shutdown();
                if (0 == ret)
                {
                    cmd_result.result_code = APB_PROXY_RESULT_OK;
                }
            }
            break;
        }

        default:
            break;
    }
#else
    cmd_result.pdata = "Not enabled";
#endif

    if (cmd_result.pdata)
    {
        cmd_result.length = strlen(cmd_result.pdata);
        ONENET_EXAMPLE_LOG("ONENETCLI pdata:%s", cmd_result.pdata);
    }
    cmd_result.cmd_id = p_parse_cmd->cmd_id;

    ONENET_EXAMPLE_LOG("ONENETCLI result_code:%d", cmd_result.result_code);
    apb_proxy_send_at_cmd_result(&cmd_result);
    return APB_PROXY_STATUS_OK;
}

