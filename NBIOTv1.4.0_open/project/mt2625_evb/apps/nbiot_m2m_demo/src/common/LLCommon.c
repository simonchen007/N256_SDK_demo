#include "LLTypeDef.h"
#include "task_def.h"
#include "hal_wdt.h"
#include "ril.h"


log_create_module(LLCOMMON, PRINT_LEVEL_DEBUG);

#define LOGE(fmt,arg...)        LOG_E(LLCOMMON, "[LLCommon.c] "fmt,##arg)
#define LOGW(fmt,arg...)        LOG_W(LLCOMMON, "[LLCommon.c] "fmt,##arg)
#define LOGI(fmt,arg...)        LOG_I(LLCOMMON, "[LLCommon.c] "fmt,##arg)

extern QueueHandle_t g_HLCopy_queue;
extern QueueHandle_t g_HLLink_queue;
extern QueueHandle_t g_HLSend_queue;
extern QueueHandle_t g_HLRecv_queue;
extern QueueHandle_t g_HLQuery_queue;
extern QueueHandle_t g_HLWatch_queue;


extern uint8_t sys_lock_handle;


unsigned char LL_send_message(msg_type msg_id, int local_para_ptr)
{
    ilm_struct ilm;	
	QueueHandle_t current_queue;

    //LOGI("LL_send_message msg_id = 0x%x", msg_id);

    ilm.msg_id = msg_id;
    ilm.local_para_ptr = local_para_ptr;

	switch(msg_id & 0xF000)
	{
		case MSG_ID_HLCOPY_BASE:
			current_queue = g_HLCopy_queue;
			break;
		case MSG_ID_HLLINK_BASE:
			current_queue = g_HLLink_queue;
			break;	
		case MSG_ID_HLSEND_BASE:
			current_queue = g_HLSend_queue;
			break;
		case MSG_ID_HLRECV_BASE:
			current_queue = g_HLRecv_queue;
			break;
		case MSG_ID_HLQUERY_BASE:
			current_queue = g_HLQuery_queue;
			break;
		case MSG_ID_HLWATCH_BASE:
			current_queue = g_HLWatch_queue;
			break;
		
		default:
			current_queue = NULL;
	}

	if(current_queue == NULL){
		return false;
	}

	if (xQueueSend(current_queue, &ilm, portMAX_DELAY) != pdPASS) {
		LOGE("xQueueSend != pdPASS");
		return false;
	}

    return true;
}

unsigned char LL_send_message_para(msg_type msg_id, int local_para_ptr, int len)
{
    ilm_struct ilm;	
	QueueHandle_t current_queue;

    //LOGI("LL_send_message msg_id = 0x%x", msg_id);

    ilm.msg_id = msg_id;
    ilm.local_para_ptr = local_para_ptr;
	ilm.local_len = len;

	switch(msg_id & 0xF000)
	{
		case MSG_ID_HLCOPY_BASE:
			current_queue = g_HLCopy_queue;
			break;
		case MSG_ID_HLLINK_BASE:
			current_queue = g_HLLink_queue;
			break;	
		case MSG_ID_HLSEND_BASE:
			current_queue = g_HLSend_queue;
			break;
		case MSG_ID_HLRECV_BASE:
			current_queue = g_HLRecv_queue;
			break;
		case MSG_ID_HLQUERY_BASE:
			current_queue = g_HLQuery_queue;
			break;
		case MSG_ID_HLWATCH_BASE:
			current_queue = g_HLWatch_queue;
			break;
		
		default:
			current_queue = NULL;
	}


	if(current_queue == NULL){
		return false;
	}

	if (xQueueSend(current_queue, &ilm, portMAX_DELAY) != pdPASS) {
		LOGE("xQueueSend != pdPASS");
		return false;
	}

    return true;
}

unsigned char LL_send_message_from_isr(msg_type msg_id, int local_para_ptr, int len)
{
    ilm_struct ilm;
    BaseType_t xHigherPriorityTaskWoken;
	QueueHandle_t current_queue;

    //LOGI("msg_id = 0x%x", msg_id);

    ilm.msg_id = msg_id;
    ilm.local_para_ptr = local_para_ptr;
	ilm.local_len = len;

	switch(msg_id & 0xF000)
	{
		case MSG_ID_HLCOPY_BASE:
			current_queue = g_HLCopy_queue;
			break;
		case MSG_ID_HLLINK_BASE:
			current_queue = g_HLLink_queue;
			break;	
		case MSG_ID_HLSEND_BASE:
			current_queue = g_HLSend_queue;
			break;
		case MSG_ID_HLRECV_BASE:
			current_queue = g_HLRecv_queue;
			break;
		case MSG_ID_HLQUERY_BASE:
			current_queue = g_HLQuery_queue;
			break;
		case MSG_ID_HLWATCH_BASE:
			current_queue = g_HLWatch_queue;
			break;
		
		default:
			current_queue = NULL;
	}


	if(current_queue == NULL){
		return false;
	}

    if (xQueueSendFromISR(current_queue, &ilm, &xHigherPriorityTaskWoken) != pdPASS) {
        LOGE("xQueueSendFromISR != pdPASS");
        return false;
    }

    return true;
}

void LLdelay_ms(uint32_t ms)
{
    uint32_t i;
    
    for(i=0;i<ms;i++)
    {
        __asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
    }
}


void ResetSystem(void)
{
	hal_wdt_config_t wdt_config;
	
	wdt_config.mode = HAL_WDT_MODE_RESET;
	wdt_config.seconds = 1;
	hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
	hal_wdt_init(&wdt_config);
	hal_wdt_software_reset();
	while(1); /* waiting the system reboot */
}

uint8_t get_crc_sum(uint8_t buf[], int len)
{
	int i = 0;
	uint8_t crc_sum = 0;
	
	for(i=0; i< len; i++)
	{
		crc_sum = crc_sum + buf[i];
	}
	return (0x100 - crc_sum);
}


void rin_lock_sys(void)
{
	if (sys_lock_handle == 0xFF) {
		sys_lock_handle = hal_sleep_manager_set_sleep_handle("sys_init");							
	}
	if(sys_lock_handle != 0xff)
	{
		hal_sleep_manager_acquire_sleeplock(sys_lock_handle, HAL_SLEEP_LOCK_DEEP);
	}
}

void rin_release_sys(void)
{
    if(sys_lock_handle != 0xff)
	{
		 hal_sleep_manager_release_sleeplock(sys_lock_handle, HAL_SLEEP_LOCK_DEEP);
	}
}

void print_hex(uint8_t buf[], int len)
{
	int i = 0;
	char temp[10]={0};
	hal_uart_send_dma(HAL_UART_0,"\r\n",2);
	for(i = 0; i< len; i++)
	{
		sprintf(temp,"0x%02x ",buf[i]);
		hal_uart_send_dma(HAL_UART_0,temp,strlen(temp));
	}
	hal_uart_send_dma(HAL_UART_0,"\r\n",2);
}

void enter_deepsleep(void)
{
	hal_rtc_gpio_control_t gpio_control;
	// Control RTC GPIO 0 output low by software.
	gpio_control.is_sw_control = true;
	gpio_control.is_sw_output_high = false;
	hal_rtc_configure_gpio(HAL_RTC_GPIO_0, &gpio_control);
	
	__asm volatile("cpsid i");
	hal_sleep_manager_enter_sleep_mode(HAL_SLEEP_MODE_DEEPER_SLEEP);
	__asm volatile("cpsie i");
}



#define LL_REG_IMSI_LEN 15
char g_imsi_num[30]={0};
static int32_t ll_register_query_imsi_callback(ril_cmd_response_t *rsp)
{
    uint8_t imsi_len = 0;
   
    ril_imsi_rsp_t *param = (ril_imsi_rsp_t*)rsp->cmd_param;
    if (rsp->res_code == RIL_RESULT_CODE_OK && param!= NULL){
        
        imsi_len = strlen(param->imsi);
        printf("[LL info]param->imsi = %s,%d\r\n", param->imsi, imsi_len);
		memset(g_imsi_num,0,sizeof(g_imsi_num));
		memcpy(g_imsi_num, param->imsi, imsi_len);
    } else {
        printf("[LL info]Query imsi result fail!\r\n");
    }
    return 0;
}

void ll_get_imsi(void)
{
	ril_status_t ret = RIL_STATUS_SUCCESS;
	ret = ril_request_imsi(RIL_ACTIVE_MODE, ll_register_query_imsi_callback, NULL);
    if (ret != RIL_STATUS_SUCCESS) {
            printf("[LL info]Query IMSI fail!\r\n");
    }
}


#define LL_REG_ICCID_LEN 20
char g_iccid_num[30]={0};
static int32_t ll_register_query_iccid_callback(ril_cmd_response_t *rsp)
{
    uint32_t iccid_len;
    ril_read_usim_iccid_rsp_t *param = (ril_read_usim_iccid_rsp_t *)rsp->cmd_param;
    
    printf("[LL info]ll_register_query_iccid_callback\r\n");
    if (rsp->res_code == RIL_RESULT_CODE_OK && param != NULL) {
        
        printf("[LL info]res:%d,param->iccid:%s!\r\n",rsp->res_code, param->iccid);
        iccid_len = strlen(param->iccid);
        memcpy(g_iccid_num, param->iccid, (iccid_len > LL_REG_ICCID_LEN)? LL_REG_ICCID_LEN : iccid_len);
    }

    return 0;
}

void ll_get_iccid(void)
{
	ril_status_t ret = RIL_STATUS_SUCCESS;
    ret = ril_request_read_usim_iccid(RIL_ACTIVE_MODE, ll_register_query_iccid_callback, NULL);
    //printf("[LL info]auto_register_read_iccid,ret:%d\r\n", ret);

    if (ret != RIL_STATUS_SUCCESS) {
        printf("[LL info]Query iccid fail!\r\n");
    }
}

