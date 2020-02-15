
#include "LLTypeDef.h"
#include "task_def.h"
#include "ril.h"
#include "hal_rtc_internal.h"
#include "apb_proxy.h"
#include "hal_wdt.h"
#include "nvdm.h"
#include "app_common_header.h"
#include "hal.h"

log_create_module(HLWATCH, PRINT_LEVEL_DEBUG);

#define LOGE(fmt,arg...)        LOG_E(HLWATCH, "[HLWatchTask.c] "fmt,##arg)
#define LOGW(fmt,arg...)        LOG_W(HLWATCH, "[HLWatchTask.c] "fmt,##arg)
#define LOGI(fmt,arg...)        LOG_I(HLWATCH, "[HLWatchTask.c] "fmt,##arg)

QueueHandle_t g_HLWatch_queue;
TaskHandle_t  g_HLWatch_task_handle;



static void HLWatch_task_main(void *arg)
{
    ilm_struct current_ilm;
	ril_status_t ret;



    while (1){
        if (xQueueReceive(g_HLWatch_queue, (void *)&current_ilm, portMAX_DELAY))
		{
			switch (current_ilm.msg_id) 
			{

				case MSG_ID_HLWATCH_AT_1_REQ: // AT+CGATT?


					break;
				
				case MSG_ID_HLWATCH_AT_2_REQ:


					break;

			}
		     
            //if (current_ilm.local_para_ptr != NULL) {
                //vPortFree(current_ilm.local_para_ptr);
           // }
        }
    }
}

void HLWatch_task_init(void)
{
	
	g_HLWatch_queue = xQueueCreate(HLWATCH_QUEUE_LENGTH, sizeof(ilm_struct));

    xTaskCreate(HLWatch_task_main,
                HLWATCH_TASK_NAME,
                HLWATCH_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                NULL,
                HLWATCH_TASK_PRIO,
                &g_HLWatch_task_handle);

	LL_ASSERT(g_HLWatch_queue != NULL);
	LL_ASSERT(g_HLWatch_task_handle != NULL);
}

