
#include "LLTypeDef.h"
#include "task_def.h"
#include "ril.h"
#include "hal_rtc_internal.h"
#include "apb_proxy.h"
#include "hal_wdt.h"
#include "nvdm.h"
#include "app_common_header.h"
#include "hal.h"

log_create_module(HLSEND, PRINT_LEVEL_DEBUG);

#define LOGE(fmt,arg...)        LOG_E(HLSEND, "[HLSendTask.c] "fmt,##arg)
#define LOGW(fmt,arg...)        LOG_W(HLSEND, "[HLSendTask.c] "fmt,##arg)
#define LOGI(fmt,arg...)        LOG_I(HLSEND, "[HLSendTask.c] "fmt,##arg)

QueueHandle_t g_HLSend_queue;
TaskHandle_t  g_HLSend_task_handle;



static void HLSend_task_main(void *arg)
{
    ilm_struct current_ilm;
	ril_status_t ret;



    while (1){
        if (xQueueReceive(g_HLSend_queue, (void *)&current_ilm, portMAX_DELAY))
		{
			switch (current_ilm.msg_id) 
			{

				case MSG_ID_HLSEND_AT_1_REQ: // AT+CGATT?


					break;
				
				case MSG_ID_HLSEND_AT_2_REQ:


					break;

			}
		     
            //if (current_ilm.local_para_ptr != NULL) {
                //vPortFree(current_ilm.local_para_ptr);
           // }
        }
    }
}

void HLSend_task_init(void)
{
	
	g_HLSend_queue = xQueueCreate(HLSEND_QUEUE_LENGTH, sizeof(ilm_struct));

    xTaskCreate(HLSend_task_main,
                HLSEND_TASK_NAME,
                HLSEND_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                NULL,
                HLSEND_TASK_PRIO,
                &g_HLSend_task_handle);

	LL_ASSERT(g_HLSend_queue != NULL);
	LL_ASSERT(g_HLSend_task_handle != NULL);
}

