
#include "LLTypeDef.h"
#include "task_def.h"
#include "ril.h"
#include "hal_rtc_internal.h"
#include "apb_proxy.h"
#include "hal_wdt.h"
#include "nvdm.h"
#include "app_common_header.h"
#include "hal.h"

log_create_module(HLRECV, PRINT_LEVEL_DEBUG);

#define LOGE(fmt,arg...)        LOG_E(HLRECV, "[HLRecvTask.c] "fmt,##arg)
#define LOGW(fmt,arg...)        LOG_W(HLRECV, "[HLRecvTask.c] "fmt,##arg)
#define LOGI(fmt,arg...)        LOG_I(HLRECV, "[HLRecvTask.c] "fmt,##arg)

QueueHandle_t g_HLRecv_queue;
TaskHandle_t  g_HLRecv_task_handle;



static void HLRecv_task_main(void *arg)
{
    ilm_struct current_ilm;
	ril_status_t ret;



    while (1){
        if (xQueueReceive(g_HLRecv_queue, (void *)&current_ilm, portMAX_DELAY))
		{
			switch (current_ilm.msg_id) 
			{

				case MSG_ID_HLRECV_AT_1_REQ: // AT+CGATT?


					break;
				
				case MSG_ID_HLRECV_AT_2_REQ:


					break;

			}
		     
            //if (current_ilm.local_para_ptr != NULL) {
                //vPortFree(current_ilm.local_para_ptr);
           // }
        }
    }
}

void HLRecv_task_init(void)
{
	
	g_HLRecv_queue = xQueueCreate(HLRECV_QUEUE_LENGTH, sizeof(ilm_struct));

    xTaskCreate(HLRecv_task_main,
                HLRECV_TASK_NAME,
                HLRECV_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                NULL,
                HLRECV_TASK_PRIO,
                &g_HLRecv_task_handle);

	LL_ASSERT(g_HLRecv_queue != NULL);
	LL_ASSERT(g_HLRecv_task_handle != NULL);
}

