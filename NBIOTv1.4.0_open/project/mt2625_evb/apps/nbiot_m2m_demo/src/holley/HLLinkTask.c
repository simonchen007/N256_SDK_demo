
#include "LLTypeDef.h"
#include "task_def.h"
#include "ril.h"
#include "hal_rtc_internal.h"
#include "apb_proxy.h"
#include "hal_wdt.h"
#include "nvdm.h"
#include "app_common_header.h"
#include "hal.h"

log_create_module(HLLINK, PRINT_LEVEL_DEBUG);

#define LOGE(fmt,arg...)        LOG_E(HLLINK, "[HLLinkTask.c] "fmt,##arg)
#define LOGW(fmt,arg...)        LOG_W(HLLINK, "[HLLinkTask.c] "fmt,##arg)
#define LOGI(fmt,arg...)        LOG_I(HLLINK, "[HLLinkTask.c] "fmt,##arg)

extern uint8_t 		tcp_pan_scoket_disconnect_flag;
extern int 			tcp_pan_socket_client;
extern TaskHandle_t PanxScoketHandle;
extern char 		g_imsi_num[];
extern char         g_iccid_num[];
extern int 			ll_tcp_socket_receive(void *not_used);


QueueHandle_t g_HLLink_queue;
TaskHandle_t  g_HLLink_task_handle;
TimerHandle_t HLMinuteTimer = NULL;
HLLink_State  HLLink_status = HLLink_net_disconnect;

void HLMinuteTimer_callback(TimerHandle_t pxTimer)
{
	if(HLLink_status == HLLink_net_disconnect)
	{
		LL_send_message(MSG_ID_HLLINK_AT_CGATT_REQ,NULL);
	}
	if(HLLink_status == HLLink_net_connected)
	{
		LL_send_message(MSG_ID_HLLINK_AT_TCP_CREATE_REQ,NULL);
	}
	
	if(HLLink_status == HLLink_tcp_created)
	{
		LL_send_message(MSG_ID_HLLINK_AT_TCP_SEND_REQ,NULL);
	}
	if(HLLink_status == HLLink_tcp_sended)
	{
		LL_send_message(MSG_ID_HLLINK_AT_TCP_CLOSE_REQ,NULL);
	}
	if(HLLink_status == HLLink_toidle)
	{
		LL_send_message(MSG_ID_HLLINK_AT_TO_IDLE_REQ,NULL);
	}

}

static int32_t HLLink_ps_attach_or_detach_callback(ril_cmd_response_t *response)
{
	static int attach_cnt = 0;

	if (!response)
	{
		return 0;
	}
	
	ril_ps_attach_or_detach_rsp_t *rsp=(ril_ps_attach_or_detach_rsp_t *)response->cmd_param;
	int32_t state = rsp->state;
	
	if(1 == state){ // attached
		HLLink_status = HLLink_net_connected;
		attach_cnt = 0;
	}else{
		attach_cnt++;
		if(attach_cnt>120){
			attach_cnt = 0;
			// more than 2 miniter cannot connect,reset
			ResetSystem();
		}
	}
	return 0;
}


static void HLLink_task_main(void *arg)
{	
	int 			socket_state = 0;
    ilm_struct 		current_ilm;
	ril_status_t 	ret;

	HLMinuteTimer = xTimerCreate("HLMinuteTimer", /* Just a text name, not used by the kernel. */
		(2000 / portTICK_PERIOD_MS), /* The timer period in ticks. */
		pdTRUE, /* The timers will auto-reload themselves when they expire. */
		NULL, /* Assign each timer a unique id equal to its array index. */
		HLMinuteTimer_callback /* Each timer calls the same callback when it expires. */
		);
	if(HLMinuteTimer != NULL){
		xTimerStart(HLMinuteTimer, 0);
	}

	rin_lock_sys();
	ll_get_imsi();
	ll_get_iccid();

    while (1){
        if (xQueueReceive(g_HLLink_queue, (void *)&current_ilm, portMAX_DELAY))
		{
			switch (current_ilm.msg_id) 
			{

				case MSG_ID_HLLINK_AT_CGATT_REQ: // AT+CGATT?
					if ((ret = ril_request_ps_attach_or_detach(RIL_READ_MODE,RIL_OMITTED_INTEGER_PARAM,HLLink_ps_attach_or_detach_callback,NULL)) < 0) {
						;
					}
					hal_uart_send_dma(HAL_UART_0,"cgatt\r\n",7);
					
					break;
				
				case MSG_ID_HLLINK_AT_TCP_CREATE_REQ:
					
					tcp_pan_scoket_disconnect_flag = 0;
					socket_state = tcp_socket_conn(&tcp_pan_socket_client, "39.105.131.183", "9930"); 
					if(socket_state == 0)
					{
						if (pdPASS != xTaskCreate(ll_tcp_socket_receive,
								  "ll_socket",
								  1024*4 / sizeof(portSTACK_TYPE),
								  NULL,
								  1/*SOCKET_EXAMPLE_TASK_PRIO*/,
								  &PanxScoketHandle)) {
							printf("Cannot create socket_sample_task\r\n");

						}
						HLLink_status = HLLink_tcp_created;
					}else{
						HLLink_status = HLLink_net_connected;
					}

					hal_uart_send_dma(HAL_UART_0,"creat\r\n",7);
					break;
				case MSG_ID_HLLINK_AT_TCP_SEND_REQ:
					{
						char send_buffer[100]={0};
						memset(send_buffer, 0, sizeof(send_buffer));
						sprintf(send_buffer,"IMSI:%s;ICCID:%s;",g_imsi_num,g_iccid_num);
						ll_tcp_send_cmd(send_buffer,strlen(send_buffer));
				    }
					HLLink_status = HLLink_tcp_sended;
					hal_uart_send_dma(HAL_UART_0,"send \r\n",7);
					break;
				case MSG_ID_HLLINK_AT_TCP_RECV_ACK:

					hal_uart_send_dma(HAL_UART_0,"r_ack\r\n",7);
					break;
				case MSG_ID_HLLINK_AT_TCP_CLOSE_REQ:

					HLLink_status = HLLink_toidle;
					hal_uart_send_dma(HAL_UART_0,"close\r\n",7);
					break;
				case MSG_ID_HLLINK_AT_UDP_CREATE_REQ:
					break;
				case MSG_ID_HLLINK_AT_UDP_SEND_REQ:
					break;
				case MSG_ID_HLLINK_AT_UDP_RECV_ACK:
					break;
				case MSG_ID_HLLINK_AT_UDP_CLOSE_REQ:
					break;
				case MSG_ID_HLLINK_AT_TO_IDLE_REQ:
					
					hal_uart_send_dma(HAL_UART_0,"IDLE \r\n",7);
					tcp_pan_scoket_disconnect_flag = 1;
					HLLink_status = HLLink_idleing;
					
					rin_release_sys();
					ril_request_set_phone_functionality(RIL_EXECUTE_MODE,0,RIL_OMITTED_INTEGER_PARAM,NULL,NULL);//AT+CFUN=0
					break;

			}
		     
            //if (current_ilm.local_para_ptr != NULL) {
                //vPortFree(current_ilm.local_para_ptr);
           // }
        }
    }
}

void HLLink_task_init(void)
{
	
	g_HLLink_queue = xQueueCreate(HLLINK_QUEUE_LENGTH, sizeof(ilm_struct));

    xTaskCreate(HLLink_task_main,
                HLLINK_TASK_NAME,
                HLLINK_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                NULL,
                HLLINK_TASK_PRIO,
                &g_HLLink_task_handle);

	LL_ASSERT(g_HLLink_queue != NULL);
	LL_ASSERT(g_HLLink_task_handle != NULL);
}
