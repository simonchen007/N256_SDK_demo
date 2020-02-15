
#include "LLTypeDef.h"
#include "task_def.h"
#include "ril.h"
#include "hal_rtc_internal.h"
#include "apb_proxy.h"
#include "hal_wdt.h"
#include "nvdm.h"
#include "app_common_header.h"
#include "hal.h"

log_create_module(LLDEVICE, PRINT_LEVEL_DEBUG);

#define LOGE(fmt,arg...)        LOG_E(LLDEVICE, "[LLDeviceTask.c] "fmt,##arg)
#define LOGW(fmt,arg...)        LOG_W(LLDEVICE, "[LLDeviceTask.c] "fmt,##arg)
#define LOGI(fmt,arg...)        LOG_I(LLDEVICE, "[LLDeviceTask.c] "fmt,##arg)

QueueHandle_t g_LLdevice_queue;
TaskHandle_t  g_LLDevice_task_handle;
extern nb_app_context g_nb_app_context;

static hal_eint_number_t switch_irq_num;
static ATTR_RWDATA_IN_RETSRAM uint32_t ll_rtc_handle=NULL;
static ATTR_RWDATA_IN_RETSRAM uint32_t count=0;
int rct_start_flag = 0;


static Lwm2m_State lwm2m_status_client = Lwm2m_net_disconnect;

int g_sim_state = 0;


int LL_get_auth_code(char deviceNum[])
{
	uint32_t length_nv = 0;
	nvdm_status_t status;
	char buffer[50] = {0};
	length_nv = sizeof(buffer);
	status = nvdm_read_data_item("RinlinkAuth", "authcode", buffer, &length_nv);
	if (NVDM_STATUS_OK != status) {
		 strcpy(deviceNum, "A062300001");
	   printf("\r\nfail RinlinkAuth authcode\r\n");
	}else{
	   printf("\r\n-------------------------- authcode = %s\r\n",buffer);	
	   memset(deviceNum,0,sizeof(deviceNum));
	   strcpy(deviceNum, buffer);
	}
	return length_nv;
}

int LL_get_serviceip_code(char serviceip[])
{
	uint32_t length_nv = 0;
	nvdm_status_t status;
	char buffer[50] = {0};
	length_nv = sizeof(buffer);
	status = nvdm_read_data_item("RinlinkAuth", "serviceip", buffer, &length_nv);
	if (NVDM_STATUS_OK != status) {
	   printf("\r\nfail RinlinkAuth serviceip\r\n");
	}else{
	   printf("\r\n-------------------------- serviceip = %s\r\n",buffer);	
	   memset(serviceip,0,sizeof(serviceip));
	   strcpy(serviceip, buffer);
	}
	return length_nv;
}

static int32_t ps_attach_or_detach_cb(ril_cmd_response_t *response)
{
	static int attach_cnt = 0;

	if (!response)
	{
		return 0;
	}
	
	ril_ps_attach_or_detach_rsp_t *rsp=(ril_ps_attach_or_detach_rsp_t *)response->cmd_param;
	int32_t state = rsp->state;
	
	if(1 == state){ // attached
		lwm2m_status_client = Lwm2m_net_connected;
		attach_cnt = 0;
	}else{
		attach_cnt++;
		if(attach_cnt>180){
			attach_cnt = 0;
			// more than 3 miniter cannot connect,reset
			ResetSystem();
		}
	}
	return 0;
}

char ll_cesq[100] = {0};
static int32_t LL_register_query_cesq_callback(ril_cmd_response_t *cmd_response)
{
	ril_extended_signal_quality_rsp_t *response = (ril_extended_signal_quality_rsp_t *)cmd_response->cmd_param;
	if (cmd_response->res_code == RIL_RESULT_CODE_OK && response != NULL) {
		printf("rxlev: %d\r\n", (int) response->rxlev);
		printf("ber: %d\r\n", (int) response->ber);
		printf("rscp: %d\r\n", (int) response->rscp);
		printf("ecno: %d\r\n", (int) response->ecno);
		printf("rsrq: %d\r\n", (int) response->rsrq);
		printf("rsrp: %d\r\n", (int) response->rsrp);
		
		sprintf(ll_cesq,"rxlev:%d,ber:%d,rscp:%d,ecno:%d,rsrq:%d,rsrp:%d\r\n",response->rxlev,response->ber,response->rscp,response->ecno,response->rsrq,response->rsrp);
	//	hal_uart_send_dma(HAL_UART_0,ll_cesq,strlen(ll_cesq));
	}
	return 0;
}

char ll_imei[50]={0};
static int32_t LL_register_query_imei_callback(ril_cmd_response_t *rsp)
{
    ril_status_t ret = RIL_STATUS_SUCCESS;
	char temp[50]={0};
    uint8_t imei_len = 0;
	char ll_imei_flag = 0;
	nvdm_status_t status;
	int i=0;
	
    ril_serial_number_rsp_t *param = (ril_serial_number_rsp_t*)rsp->cmd_param;

    if (rsp->res_code == RIL_RESULT_CODE_OK && param!= NULL){
        
        printf("[LL_reg]rsp->res_code = %d\r\n", rsp->res_code);
        imei_len = strlen(param->value.imei);

        printf("[LL_reg]param->value.imei = %s,%d\r\n", param->value.imei, imei_len); 
		memset(ll_imei, 0, sizeof(ll_imei));
        memcpy(ll_imei, param->value.imei, imei_len);
		printf("imei ----------- =%s\r\n",ll_imei);
		hal_uart_send_dma(HAL_UART_0,ll_imei,strlen(ll_imei));
		for(i=0; i< imei_len; i++)
		{
			if(ll_imei[i] != '0' )
				ll_imei_flag = 1;
		}
		if(ll_imei_flag)
		{
			memset(temp, 0, sizeof(temp));
			temp[0] = '1';
			status = nvdm_write_data_item("RinlinkFlag", "FIMEI", NVDM_DATA_ITEM_TYPE_STRING, temp, strlen((char *)temp));
			if (NVDM_STATUS_OK != status) {
			   printf("\r\n fail RinlinkFlag FIMEI \r\n");
			}else{
			   printf("\r\n w-------------------------- RinlinkFlag FIMEI = %s\r\n",temp);	
			}
		}else{
			memset(temp, 0, sizeof(temp));
			temp[0] = '0';
			status = nvdm_write_data_item("RinlinkFlag", "FIMEI", NVDM_DATA_ITEM_TYPE_STRING, temp, strlen((char *)temp));
			if (NVDM_STATUS_OK != status) {
			   printf("\r\n fail RinlinkFlag FIMEI \r\n");
			}else{
			   printf("\r\n w-------------------------- RinlinkFlag FIMEI = %s\r\n",temp);	
			}
		}
			
    } else {
        printf("[LL_reg]Query imei result fail!\r\n");

    }
    return 0;
}

#define LL_REG_IMSI_LEN 15
char g_imsi_num[50]={0};
static int32_t ll_register_query_imsi_callback(ril_cmd_response_t *rsp)
{
    uint8_t imsi_len = 0;
   
    ril_imsi_rsp_t *param = (ril_imsi_rsp_t*)rsp->cmd_param;
    if (rsp->res_code == RIL_RESULT_CODE_OK && param!= NULL){
        
        imsi_len = strlen(param->imsi);
        printf("[LL info]param->imsi = %s,%d\r\n", param->imsi, imsi_len);
		memset(g_imsi_num,0,sizeof(g_imsi_num));
		memcpy(g_imsi_num, param->imsi, imsi_len);
		g_sim_state = 1;
		//hal_uart_send_dma(HAL_UART_0,"SIM1\r\n",6);
    } else {
		g_sim_state = 0;
		//hal_uart_send_dma(HAL_UART_0,"SIM0\r\n",6);
        printf("[LL info]Query imsi result fail!\r\n");

    }
    return 0;
}
void ll_get_imsi()
{
	ril_status_t ret = RIL_STATUS_SUCCESS;
	ret = ril_request_imsi(RIL_ACTIVE_MODE, ll_register_query_imsi_callback, NULL);
    if (ret != RIL_STATUS_SUCCESS) {
            printf("[LL info]Query IMSI fail!\r\n");
    }
}

TimerHandle_t xTimerpass_sleep = NULL;
void test_sleep(TimerHandle_t pxTimer)
{
	printf("\r\n --------------  timer sleep----------------------------\r\n");
	//ril_request_set_phone_functionality(RIL_EXECUTE_MODE,0,RIL_OMITTED_INTEGER_PARAM,NULL,NULL);//AT+CFUN=0
	LL_send_message(MSG_ID_LLDEVICE_DEEPSLEEP,NULL);	
	xTimerpass_sleep = NULL;
}


void start_sleep_timer()
{
	if(xTimerpass_sleep == NULL)
	{
		xTimerpass_sleep = xTimerCreate("Timer_sleep", /* Just a text name, not used by the kernel. */
						(1000 / portTICK_PERIOD_MS), /* The timer period in ticks. */
						pdFALSE, /* The timers will auto-reload themselves when they expire. */
						NULL, /* Assign each timer a unique id equal to its array index. */
						test_sleep /* Each timer calls the same callback when it expires. */
						);
		xTimerStart(xTimerpass_sleep, 0);
	}
}
void stop_sleep_timer()
{
	if(xTimerpass_sleep != NULL)
		xTimerStop(xTimerpass_sleep, 0);
	xTimerpass_sleep = NULL;
}

TimerHandle_t xTimerpass_longsleep = NULL;
void test_longsleep(TimerHandle_t pxTimer)
{
	printf("\r\n --------------  timer sleep----------------------------\r\n");
	//ril_request_set_phone_functionality(RIL_EXECUTE_MODE,0,RIL_OMITTED_INTEGER_PARAM,NULL,NULL);//AT+CFUN=0
	LL_send_message(MSG_ID_LLDEVICE_DEEPSLEEP,NULL);	
	xTimerpass_longsleep = NULL;
}
void start_longsleep_timer()
{
	if(xTimerpass_longsleep == NULL)
	{
		xTimerpass_longsleep = xTimerCreate("Timer_longsleep", /* Just a text name, not used by the kernel. */
						(120000 / portTICK_PERIOD_MS), /* The timer period in ticks. */
						pdFALSE, /* The timers will auto-reload themselves when they expire. */
						NULL, /* Assign each timer a unique id equal to its array index. */
						test_longsleep /* Each timer calls the same callback when it expires. */
						);
		xTimerStart(xTimerpass_longsleep, 0);
	}
}
void stop_longsleep_timer()
{
	if(xTimerpass_longsleep != NULL)
		xTimerStop(xTimerpass_longsleep, 0);
	xTimerpass_longsleep = NULL;
}

void test_alartsend(TimerHandle_t pxTimer)
{
	printf("\r\n --------------  timer sleep----------------------------\r\n");
	//ril_request_set_phone_functionality(RIL_EXECUTE_MODE,0,RIL_OMITTED_INTEGER_PARAM,NULL,NULL);//AT+CFUN=0
	LL_send_message(MSG_ID_LLDEVICE_AT_LWM2M_SEND,NULL);	
}


TimerHandle_t xTimerpass_test = NULL;
uint32_t count_timer = 0;
uint32_t count_timer_send_wait = 0;
extern ATTR_ZIDATA_IN_RETSRAM uint32_t app_rtc_handle;
//void rtcTimerCallbackFun(void *param);
uint32_t g_ping_packet_flag = 0;

void process_timer(TimerHandle_t pxTimer)
{
	printf("\r\n -----------------------  ------------------  ----------lwm2m_status_client=%d, count_timer =%d, g_nb_app_context.app_state=%d\r\n",lwm2m_status_client,count_timer,g_nb_app_context.app_state);
	count_timer ++;

	if(g_nb_app_context.lwm2m_info != NULL && g_nb_app_context.lwm2m_info->state == STATE_READY && lwm2m_status_client == Lwm2m_server_connecting)
	{
		lwm2m_status_client = Lwm2m_server_connected;
	}
	
	if(lwm2m_status_client == Lwm2m_net_disconnect)
	{
		LL_send_message(MSG_ID_LLDEVICE_AT_CGATT_REQ,NULL);
	}
	if(lwm2m_status_client == Lwm2m_net_connected)
	{
		LL_send_message(MSG_ID_LLDEVICE_AT_LWM2M_REQ,NULL);
		lwm2m_status_client = Lwm2m_server_connecting;
	}
	if((lwm2m_status_client == Lwm2m_server_connected) && (count_timer >= count_timer_send_wait))
	{
		LL_send_message(MSG_ID_LLDEVICE_AT_LWM2M_SEND,NULL);
		count_timer_send_wait = count_timer + 10;
	}
	if(lwm2m_status_client == Lwm2m_send)
	{
		LL_send_message(MSG_ID_LLDEVICE_AT_IDLE,NULL);
		count_timer_send_wait = 0;
		count_timer = 0;
	}
	//if(lwm2m_status_client == Lwm2m_low_power)
	//{
	//	printf("\r\n ------------ stop time ---------------------\r\n");
	//	xTimerStop(pxTimer, 0);
	//	count_timer_send_wait = 0;
		//xTimerDelete(pxTimer, portMAX_DELAY);
		//rtc_sw_timer_start(app_rtc_handle);
		//rtc_sw_timer_create(&app_rtc_handle, NOTIFICATION_UPDATE_TIME, true, rtcTimerCallbackFun);
	//}
	
}

char g_send_buf[1024]={0};

void get_system_info()
{
	//ril_request_serial_number(RIL_EXECUTE_MODE, 1, LL_register_query_imei_callback, NULL);
	//ril_request_signal_quality(RIL_ACTIVE_MODE, LL_register_query_csq_callback, NULL);
	//ril_request_extended_signal_quality(RIL_ACTIVE_MODE, LL_register_query_cesq_callback, NULL);
	//ril_request_query_network_state(RIL_EXECUTE_MODE, 0, LL_register_query_meminfo_callback, "sclp");
	//ll_get_iccid();
	ll_get_imsi();
}

void LL_wakeup_timer_callback(void *user_data)
{
	LOGI("=====TF===== LL_wakeup_timer_callback count=%d \r\n",count++);
	rct_start_flag = 0;
	if(ll_rtc_handle != NULL){
		//rtc_sw_timer_stop(ll_rtc_handle);
	}
}
#define LL_TIMER_ONE_SECOND	10
void LL_start_rtc(uint32_t sec_time)
{
	uint32_t rtc_time = 0;
	rtc_time = sec_time*LL_TIMER_ONE_SECOND;
	if(rct_start_flag == 0)
	{
		rtc_sw_timer_create(&ll_rtc_handle, rtc_time, false, LL_wakeup_timer_callback);
		//rtc_sw_timer_stop(ll_rtc_handle);
		if(ll_rtc_handle != NULL){
			rtc_sw_timer_start(ll_rtc_handle);
			rct_start_flag = 1;
		}else{
			ResetSystem();
		}
	}	
}

#define LWM2M_REG_COUNT 3
int lwm2m_register_count = 0;
int lwm2m_send_count = 0;
int lwm2m_alart_flag = 0;
int lwm2m_alarttimer_flag = 0;
int lwm2m_sendend_flag = 0;

static void LLDevice_task_main(void *arg)
{
	hal_spi_master_status_t status;
	hal_spi_master_send_and_receive_config_t spi_send_and_receive_config;
    ilm_struct current_ilm;
	ril_status_t ret;
	lwm2m_creat lwm2m_creat_p= {"180.101.147.115", "5683", "862174031508484","6000","",""}; //2222222222220000004
	//lwm2m_creat lwm2m_creat_p= {"117.60.157.137", "5683", "861659064652002","6000","",""}; //2222222222220000004

	rin_lock_sys();
	
	xTimerpass_test = xTimerCreate("Timer_memory", /* Just a text name, not used by the kernel. */
		(2000 / portTICK_PERIOD_MS), /* The timer period in ticks. */
		pdTRUE, /* The timers will auto-reload themselves when they expire. */
		NULL, /* Assign each timer a unique id equal to its array index. */
		process_timer /* Each timer calls the same callback when it expires. */
		);
	if(xTimerpass_test != NULL){
		xTimerStart(xTimerpass_test, 0);
	}
			
	vTaskDelay(1000 / portTICK_RATE_MS);
	ril_request_set_phone_functionality(RIL_EXECUTE_MODE,1,RIL_OMITTED_INTEGER_PARAM,NULL,NULL);//AT+CFUN=1
	ril_request_serial_number(RIL_EXECUTE_MODE, 1, LL_register_query_imei_callback, NULL);
	
    while (1){
        if (xQueueReceive(g_LLdevice_queue, (void *)&current_ilm, portMAX_DELAY))
		{
			switch (current_ilm.msg_id) 
			{
				case MSG_ID_LLDEVICE_UART_REQ:
					
					break;
				case MSG_ID_LLDEVICE_AT_CGATT_REQ: // AT+CGATT?

					if ((ret = ril_request_ps_attach_or_detach(RIL_READ_MODE,RIL_OMITTED_INTEGER_PARAM,ps_attach_or_detach_cb,NULL)) < 0) {
						;
					}
					hal_uart_send_dma(HAL_UART_0,"gatt\r\n",6);
					
					break;
				
				case MSG_ID_LLDEVICE_AT_LWM2M_REQ:

						memset(lwm2m_creat_p.deviceID, 0, sizeof(lwm2m_creat_p.deviceID));
						memcpy(lwm2m_creat_p.deviceID, ll_imei, strlen(ll_imei));
						
						LL_get_serviceip_code(lwm2m_creat_p.ip);
						hal_uart_send_dma(HAL_UART_0,lwm2m_creat_p.ip,15);
						
						apb_proxy_hdlr_lwm2m_client_create(lwm2m_creat_p);
						hal_uart_send_dma(HAL_UART_0,"create\r\n",strlen("create\r\n"));
						hal_uart_send_dma(HAL_UART_0,lwm2m_creat_p.deviceID,strlen(lwm2m_creat_p.deviceID));
						
						get_system_info();
						
					break;
				case MSG_ID_LLDEVICE_AT_LWM2M_SEND:

					memcpy(g_send_buf,"123456789",strlen("123456789"));
					apb_proxy_hdlr_lwm2m_client_send(g_send_buf, strlen(g_send_buf));
					hal_uart_send_dma(HAL_UART_0,"send\r\n",6);
					lwm2m_status_client = Lwm2m_send;
					break;

				case MSG_ID_LLDEVICE_AT_IDLE:
					
					apb_proxy_hdlr_lwm2m_client_delete();
					
					rin_release_sys();
					
					hal_uart_send_dma(HAL_UART_0,"lowp\r\n",6);
					lwm2m_status_client = Lwm2m_low_power;
					xTimerStop(xTimerpass_test, 0);				
					start_sleep_timer();
					break;
				case MSG_ID_LLDEVICE_DEEPSLEEP:
					rin_release_sys();
					lwm2m_status_client = Lwm2m_net_disconnect;
					hal_uart_send_dma(HAL_UART_0,"shutdown\r\n",strlen("shutdown\r\n"));
					LL_start_rtc(60);
					
					//enter_deepsleep();
					break;
			}
		     
            //if (current_ilm.local_para_ptr != NULL) {
                //vPortFree(current_ilm.local_para_ptr);
           // }
        }
    }
}

void LLdevice_task_init(void)
{
	
	g_LLdevice_queue = xQueueCreate(LLDEVICE_QUEUE_LENGTH, sizeof(ilm_struct));

    xTaskCreate(LLDevice_task_main,
                LLDEVICE_TASK_NAME,
                LLDEVICE_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                NULL,
                LLDEVICE_TASK_PRIO,
                &g_LLDevice_task_handle);

	LL_ASSERT(g_LLdevice_queue != NULL);
	LL_ASSERT(g_LLDevice_task_handle != NULL);
}

