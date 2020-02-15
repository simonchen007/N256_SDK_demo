/* Copyright Statement:
 *
 * (C) 2005-2017  MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
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

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* device.h includes */
#include "mt2625.h"

/* hal includes */
#include "hal.h"
#include "hal_rtc_internal.h"
#include "memory_attribute.h"

#include "nvdm.h"
#ifdef MTK_N301_PRJ_NAME
#include "serial_port.h"
#include "td1030.h"
#endif
#include "sys_init.h"
#include "task_def.h"
#include "tel_conn_mgr_app_api.h"
#include "ril_task.h"
#ifdef MTK_FOTA_ENABLE
#include "fota.h"
#include "apb_proxy_fota_cmd.h"
#endif
#ifdef MTK_USB_DEMO_ENABLED
#include "usb.h"
#endif

#ifdef __RS_FOTA_SUPPORT__
#include "rs_sdk_api.h"
#endif

//#include "sensor_demo.h"
#ifdef SENSOR_DEMO
#include "sensor_alg_interface.h"
#endif

#ifdef MTK_GNSS_ENABLE
#include "gnss_app.h"
extern void gnss_demo_main();
#endif

/* for tracing and assert function prototypes */
#include "frhsl.h"
#include "system.h"

/* mux ap includes */
#include "mux_ap.h"
/* ril includes */
#include "ril.h"
/* AP Bridge Proxy inlcudes*/
#include "apb_proxy_task.h"
#include "auto_register.h"

#include "lwip/tcpip.h"
#ifdef MTK_TCPIP_FOR_NB_MODULE_ENABLE
#include "nbnetif.h"
#endif
#include "nidd_gprot.h"
#ifdef MTK_ATCI_APB_PROXY_NETWORK_ENABLE
#include "apb_proxy_nw_cmd_gprot.h"
#endif

#include "hal_wdt.h"
#include "serial_port.h"

#include "lltypedef.h"

#ifdef HAL_KEYPAD_MODULE_ENABLED
#include "keypad_custom.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED
#include "hal_sleep_manager.h"
#include "hal_sleep_driver.h"
#include "hal_gpt.h"
#define POWER_KEY_LOCK_SLEEP_IN_SEC 10
const static char *powerkey_lock_sleep_name = "Powerkey_main";
static uint8_t powerkey_lock_sleep_handle;
static bool power_key_timer = false;
uint32_t timer_handle;
#endif
#endif

#if( configAPPLICATION_ALLOCATED_HEAP == 1 )
ATTR_NONINIT_DATA_IN_RAM uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif

extern void md_init_phase_2(void);
extern void simat_proxy_init();

#ifdef MTK_COAP_SUPPORT
extern void nw_coap_init(void);
#endif

struct assert_user_var_t global_assert_user_var_t = {0};

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//#define FREERTOS_TEST

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#if !defined (MTK_DEBUG_LEVEL_NONE)
LOG_CONTROL_BLOCK_DECLARE(atci_serialport);
LOG_CONTROL_BLOCK_DECLARE(atcmd);
LOG_CONTROL_BLOCK_DECLARE(common);
LOG_CONTROL_BLOCK_DECLARE(hal);
LOG_CONTROL_BLOCK_DECLARE(httpclient);
LOG_CONTROL_BLOCK_DECLARE(iperf);
LOG_CONTROL_BLOCK_DECLARE(ping);
LOG_CONTROL_BLOCK_DECLARE(RTC_ATCI);
LOG_CONTROL_BLOCK_DECLARE(fota_http_dl);
LOG_CONTROL_BLOCK_DECLARE(fota);
LOG_CONTROL_BLOCK_DECLARE(apb_proxy);
LOG_CONTROL_BLOCK_DECLARE(mux_ap);
LOG_CONTROL_BLOCK_DECLARE(auto_reg);


log_control_block_t *syslog_control_blocks[] = {
    &LOG_CONTROL_BLOCK_SYMBOL(atci_serialport),
    &LOG_CONTROL_BLOCK_SYMBOL(atcmd),
    &LOG_CONTROL_BLOCK_SYMBOL(common),
    &LOG_CONTROL_BLOCK_SYMBOL(hal),
    &LOG_CONTROL_BLOCK_SYMBOL(httpclient),
    &LOG_CONTROL_BLOCK_SYMBOL(iperf),
    &LOG_CONTROL_BLOCK_SYMBOL(ping),
    &LOG_CONTROL_BLOCK_SYMBOL(RTC_ATCI),
    &LOG_CONTROL_BLOCK_SYMBOL(fota_http_dl),
    &LOG_CONTROL_BLOCK_SYMBOL(fota),
    &LOG_CONTROL_BLOCK_SYMBOL(apb_proxy),
    &LOG_CONTROL_BLOCK_SYMBOL(mux_ap),
    &LOG_CONTROL_BLOCK_SYMBOL(auto_reg),
    NULL
};

static void syslog_config_save(const syslog_config_t *config)
{
    nvdm_status_t status;
    char *syslog_filter_buf;

    syslog_filter_buf = (char*)pvPortMalloc(SYSLOG_FILTER_LEN);
    configASSERT(syslog_filter_buf != NULL);
    syslog_convert_filter_val2str((const log_control_block_t **)config->filters, syslog_filter_buf);
    status = nvdm_write_data_item("common",
                                  "syslog_filters",
                                  NVDM_DATA_ITEM_TYPE_STRING,
                                  (const uint8_t *)syslog_filter_buf,
                                  strlen(syslog_filter_buf));
    vPortFree(syslog_filter_buf);
    LOG_I(common, "syslog config save, status=%d", status);
}

static uint32_t syslog_config_load(syslog_config_t *config)
{
    uint32_t sz = SYSLOG_FILTER_LEN;
    char *syslog_filter_buf;

    syslog_filter_buf = (char*)pvPortMalloc(SYSLOG_FILTER_LEN);
    configASSERT(syslog_filter_buf != NULL);
    if (nvdm_read_data_item("common", "syslog_filters", (uint8_t*)syslog_filter_buf, &sz) == NVDM_STATUS_OK) {
        syslog_convert_filter_str2val(config->filters, syslog_filter_buf);
    } else {
        /* popuplate the syslog nvdm with the image setting */
        syslog_config_save(config);
    }
    vPortFree(syslog_filter_buf);
    return 0;
}
#endif

void big_assert(void);

extern void       KiOsReset               (void);
void AssertHandlerNoReturn(unsigned flags,...)
{
	/* This is desperate trick to possible store registers before they are cleared/overrun/used */
	asm volatile ("nop" ::: "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12" );

    char *module_name = NULL;
    int line_number = 0;
    //char *function_name = NULL;
    //char *user_msg = NULL;
    uint16_t conditions = 0;
    va_list arg_list;
    //char trace_buffer[80];

    __disable_irq();

    // go to beginning of variable argument list
    va_start(arg_list, flags);

    // read argumets based on flag bits
    conditions = (uint16_t) flags;
    if (flags & AF_FLAGS_FILE_)
    {
      module_name = va_arg(arg_list, char *);
      line_number = va_arg(arg_list, int);
    }
    if (flags & AF_FLAGS_FUNC_)
    {
      //function_name = va_arg(arg_list, char *);
    }
    if (flags & AF_FLAGS_COND_)
    {
      conditions |= (1u << 8);
    }
    if (flags & AF_FLAGS_MSG_)
    {
      //user_msg = va_arg(arg_list, char *);
    }
    if (flags & AF_FLAGS_VARS_)
    {
      uint32_t loopc;
      global_assert_user_var_t.user_var_present = true;

      for(loopc = 0; loopc < 3; loopc++)
      {
        global_assert_user_var_t.user_var[loopc] = va_arg(arg_list, unsigned int);
      }
    }
    if (0 == (flags & AF_FLAGS_FATAL_))
    {
      conditions |= (1u << 9);
    }

/*
	// Trace error info to HLS
    FrHslString (0x000A, "\r\n-- AssertHandlerNoReturn called --");
    sprintf(trace_buffer, "\r\n   with Flags = %#04X",conditions);
    FrHslString (0x000A, trace_buffer);

//    M_FrHslPrintf1 (0x9515, DEFAULT_GROUP, "Assert: AssertHandlerNoReturn called. Flags=%{1}8.0b, condition checked=%{1}1.8b, error=%{1}1.9b, raw=%{1}#04X",
//      conditions)
    if (NULL != module_name)
    {
        sprintf(trace_buffer, "\r\n  Module name: %s",module_name);
        FrHslString (0x000A, trace_buffer);
        sprintf(trace_buffer, "\r\n  Line number: %lu",line_number);
        FrHslString (0x000A, trace_buffer);
//        M_FrHslString (0xCAF8, DEFAULT_GROUP, "  Module name: %s", module_name);
//        M_FrHslPrintf2 (0xBB0F, DEFAULT_GROUP, "  Line number: %lu", (uint16_t) (line_number >> 16), (uint16_t) line_number);
    }
    if (NULL != function_name)
    {
        sprintf(trace_buffer, "\r\n  Function name: %s", function_name);
        FrHslString (0x000A, trace_buffer);
//        M_FrHslString (0xCF58, DEFAULT_GROUP, "  Function name: %s", function_name);
    }

//  !!! Need to figure out still how to put this data out, will help debugging when we see this data in HSL traces as well !!!

    if (NULL != user_msg)
    {
        sprintf(trace_buffer, "\r\n  User message: %s", user_msg);
        FrHslString (0x000A, trace_buffer);
//        M_FrHslString (0xC724, DEFAULT_GROUP, "  User message: %s", user_msg);
    }

//        sprintf(trace_buffer, "\r\n  User variables: [0]=%#08lX, [1]=%#08lX, [2]=%#08lX", user_var[0], user_var[1], user_var[2]);
//        FrHslString (0x000A, trace_buffer);
//        M_FrHslPrintf6 (0xA3F8, DEFAULT_GROUP, "  User variables: [0]=%#08lX, [1]=%#08lX, [2]=%#08lX",
//        (uint16_t) (user_var[0] >> 16), (uint16_t) user_var[0],
//        (uint16_t) (user_var[1] >> 16), (uint16_t) user_var[1],
//        (uint16_t) (user_var[2] >> 16), (uint16_t) user_var[2]);
    }
*/

    // call platform assert to create full path towards memory dump
    platform_assert(0, module_name, line_number );

    // this function prototype is defined with noreturn pragma, if function returns to
    // caller system will crash in very mysterious ways.
    for(;;);
}

#define mainCHECK_DELAY ( ( portTickType ) 1000 / portTICK_RATE_MS )

#ifdef MTK_N301_PRJ_NAME
void gpio_application(void)
{
  hal_gpio_init(HAL_GPIO_20);
  hal_pinmux_set_function(HAL_GPIO_20, HAL_GPIO_20_GPIO20); // Set the pin to operate in GPIO mode.
  hal_gpio_set_direction(HAL_GPIO_20, HAL_GPIO_DIRECTION_OUTPUT);
  hal_gpio_set_output(HAL_GPIO_20, HAL_GPIO_DATA_HIGH);
  //hal_gpio_deinit(HAL_GPIO_20);
  
  hal_gpio_init(HAL_GPIO_1);
  hal_pinmux_set_function(HAL_GPIO_1, HAL_GPIO_1_GPIO1); // Set the pin to operate in GPIO mode.
  hal_gpio_set_direction(HAL_GPIO_1, HAL_GPIO_DIRECTION_OUTPUT);
  hal_gpio_set_output(HAL_GPIO_1, HAL_GPIO_DATA_LOW);
  
}

void gpio_20_low()
{
	hal_gpio_set_output(HAL_GPIO_20, HAL_GPIO_DATA_LOW);
}

void gpio_20_hight()
{
	hal_gpio_set_output(HAL_GPIO_20, HAL_GPIO_DATA_HIGH);
}

void gpio_1_low()
{
	hal_gpio_set_output(HAL_GPIO_1, HAL_GPIO_DATA_LOW);
}

void gpio_1_hight()
{
	hal_gpio_set_output(HAL_GPIO_1, HAL_GPIO_DATA_HIGH);
}

//uart 2
#define SERIAL_PORT2_RECEIVE_BUFFER_SIZE 512
#define SERIAL_PORT2_SEND_BUFFER_SIZE 512

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t g_serial_port2_receive_vfifo_buffer[SERIAL_PORT2_RECEIVE_BUFFER_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t g_serial_port2_send_vfifo_buffer[SERIAL_PORT2_SEND_BUFFER_SIZE];
char serial_receive[SERIAL_PORT2_RECEIVE_BUFFER_SIZE];

hal_uart_port_t uart_port = HAL_UART_2;
TD1030_RMC rmc_data = {0};
TD1030_GNS gns_data = {0};
TD1030_GSV gsv_dataGP = {0};
TD1030_GSV gsv_dataBD = {0};

#if 0
const char gps_data[]="$GNRMC,082608.00,A,3107.60826,N,12121.54564,E,0.251,,260718,,,A,V*14\r\n"   \
"$GNGGA,082608.00,3107.60826,N,12121.54564,E,1,05,2.01,0.9,M,9.8,M,,*70\r\n" \
"$GNGSA,A,3,19,17,02,05,12,,,,,,,,4.13,2.01,3.60,1*0B\r\n"                   \
"$GNGSA,A,3,,,,,,,,,,,,,4.13,2.01,3.60,4*04\r\n"                             \
"$GPGSV,2,1,08,02,53,331,34,05,47,257,28,06,52,059,25,07,00,104,,0*68\r\n"   \
"$GPGSV,2,2,08,12,26,262,30,17,28,148,30,19,52,149,31,25,,,21,0*58\r\n"      \
"$BDGSV,1,1,00,0*74\r\n";
#endif

unsigned int gpsdebug = 0;

static void serial_port2_uart_callback(hal_uart_callback_event_t event, void *user_data)
{
    //serial_port_dev_t uart_port = *(serial_port_dev_t *)user_data;
	int receive_length = 0;
	//hal_uart_send_dma(uart_port,"CALLBACK\r\n",10);
	if (event == HAL_UART_EVENT_READY_TO_READ) {
		memset(serial_receive,0,sizeof(serial_receive));
		receive_length = hal_uart_receive_dma(uart_port,serial_receive,sizeof(serial_receive));

		if(gpsdebug){
			hal_uart_send_dma(HAL_UART_0,serial_receive,receive_length);
		}

		if(serial_receive[3] == 'R' && serial_receive[4] == 'M' && serial_receive[5] == 'C')
		{
			//hal_uart_send_dma(HAL_UART_0,"\r\nRMC\r\n",7);
			td1030_rmc_parse(&rmc_data, serial_receive);
		}
		
		//hal_uart_send_dma(HAL_UART_0,serial_receive,receive_length);
		//hal_uart_send_dma(HAL_UART_0,rmc_data.Latitudinal,strlen(rmc_data.Latitudinal));
		
		
	} else if (event == HAL_UART_EVENT_READY_TO_WRITE) {
		
		//hal_uart_send_dma(uart_port,"WRITE\r\n",7);
	}
}

serial_port_status_t serial_port2_uart_init(void)
{
    
    hal_uart_status_t status;
    hal_uart_config_t uart_config;
    hal_uart_dma_config_t dma_config;
	hal_gpio_status_t ret = HAL_GPIO_STATUS_ERROR;
   
    //uart_port = HAL_UART_2;//device;
    
	hal_gpio_init(HAL_GPIO_31);
    hal_gpio_init(HAL_GPIO_32);
    hal_pinmux_set_function(HAL_GPIO_31, HAL_GPIO_31_UART2_RXD);
    hal_pinmux_set_function(HAL_GPIO_32, HAL_GPIO_32_UART2_TXD);
	ret = hal_gpio_pull_up(HAL_GPIO_32);
	if (HAL_GPIO_STATUS_OK != ret) {
        printf("hal_gpio_pull_up failed\r\n");
    }else{
		//printf("hal_gpio_pull_up ok\r\n");
	}
	
	#if 0
	hal_gpio_init(HAL_GPIO_16);
    hal_gpio_init(HAL_GPIO_17);
    hal_pinmux_set_function(HAL_GPIO_16, HAL_GPIO_16_UART1_RXD);
    hal_pinmux_set_function(HAL_GPIO_17, HAL_GPIO_17_UART1_TXD);
	ret = hal_gpio_pull_up(HAL_GPIO_17);
	if (HAL_GPIO_STATUS_OK != ret) {
        printf("hal_gpio_pull_up failed\r\n");
    }else{
		//printf("hal_gpio_pull_up ok\r\n");
	}
	#endif
	
    #if 0
	hal_gpio_init(HAL_GPIO_26);
    hal_gpio_init(HAL_GPIO_27);
    hal_pinmux_set_function(HAL_GPIO_26, HAL_GPIO_26_UART3_RXD);
    hal_pinmux_set_function(HAL_GPIO_27, HAL_GPIO_27_UART3_TXD);
	ret = hal_gpio_pull_up(HAL_GPIO_27);
	if (HAL_GPIO_STATUS_OK != ret) {
        printf("hal_gpio_pull_up failed\r\n");
    }else{
		//printf("hal_gpio_pull_up ok\r\n");
	}
	#endif
	
    uart_config.baudrate = HAL_UART_BAUDRATE_9600;
    uart_config.parity = HAL_UART_PARITY_NONE;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    status = hal_uart_init(uart_port, &uart_config);
    if (status != HAL_UART_STATUS_OK) {
        return SERIAL_PORT_STATUS_FAIL;
    }

    dma_config.receive_vfifo_alert_size = 20;
    dma_config.receive_vfifo_buffer = g_serial_port2_receive_vfifo_buffer;
    dma_config.receive_vfifo_buffer_size = SERIAL_PORT2_RECEIVE_BUFFER_SIZE;
    dma_config.receive_vfifo_threshold_size = (SERIAL_PORT2_RECEIVE_BUFFER_SIZE * 2) / 3;
    dma_config.send_vfifo_buffer = g_serial_port2_send_vfifo_buffer;
    dma_config.send_vfifo_buffer_size = SERIAL_PORT2_SEND_BUFFER_SIZE;
    dma_config.send_vfifo_threshold_size = SERIAL_PORT2_SEND_BUFFER_SIZE / 10;

	status = hal_uart_set_dma(uart_port, &dma_config);
    if (status != HAL_UART_STATUS_OK) {
        return SERIAL_PORT_STATUS_FAIL;
    }

    status = hal_uart_register_callback(uart_port, serial_port2_uart_callback, NULL);
    if (status != HAL_UART_STATUS_OK) {
        return SERIAL_PORT_STATUS_FAIL;
    }

    return SERIAL_PORT_STATUS_OK;
}

#endif

ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t g_serial_port1_receive_vfifo_buffer[SERIAL_PORT1_RECEIVE_BUFFER_SIZE];
ATTR_ZIDATA_IN_NONCACHED_RAM_4BYTE_ALIGN static uint8_t g_serial_port1_send_vfifo_buffer[SERIAL_PORT1_SEND_BUFFER_SIZE];

char serial1_receive[SERIAL_PORT1_RECEIVE_BUFFER_SIZE];

static void serial_port1_uart_callback(hal_uart_callback_event_t event, void *user_data)
{
	int receive_length = 0;

	if (event == HAL_UART_EVENT_READY_TO_READ) {
		memset(serial1_receive,0,sizeof(serial1_receive));
		receive_length = hal_uart_receive_dma(HAL_UART_1,serial1_receive,sizeof(serial1_receive));
		
		hal_uart_send_dma(HAL_UART_0,serial1_receive,receive_length);
		hal_uart_send_dma(HAL_UART_0,"\r\n",2);
	} 
	else if (event == HAL_UART_EVENT_READY_TO_WRITE)
    {
		//hal_uart_send_dma(HAL_UART_1,"WRITE\r\n",7);
	}
}

serial_port_status_t serial_port1_uart_init()
{
    hal_uart_port_t uart_port1;
    hal_uart_status_t status;
    hal_uart_config_t uart_config;
    hal_uart_dma_config_t dma_config;
	hal_gpio_status_t ret = HAL_GPIO_STATUS_ERROR;
   
    uart_port1 = HAL_UART_1;//device;
    
	hal_gpio_init(HAL_GPIO_16);
    hal_gpio_init(HAL_GPIO_17);
    hal_pinmux_set_function(HAL_GPIO_16, HAL_GPIO_16_UART1_RXD);
    hal_pinmux_set_function(HAL_GPIO_17, HAL_GPIO_17_UART1_TXD);
	ret = hal_gpio_pull_up(HAL_GPIO_17);
	if (HAL_GPIO_STATUS_OK != ret) {
        printf("hal_gpio_pull_up failed\r\n");
    }else{
		//printf("hal_gpio_pull_up ok\r\n");
	}
	
    uart_config.baudrate = HAL_UART_BAUDRATE_2400;
    uart_config.parity = HAL_UART_PARITY_EVEN;  //Å¼Ð£Ñé
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    status = hal_uart_init(uart_port1, &uart_config);
    if (status != HAL_UART_STATUS_OK) {
        return SERIAL_PORT_STATUS_FAIL;
    }

    dma_config.receive_vfifo_alert_size = 20;
    dma_config.receive_vfifo_buffer = g_serial_port1_receive_vfifo_buffer;
    dma_config.receive_vfifo_buffer_size = SERIAL_PORT1_RECEIVE_BUFFER_SIZE;
    dma_config.receive_vfifo_threshold_size = (SERIAL_PORT1_RECEIVE_BUFFER_SIZE * 2) / 3;
    dma_config.send_vfifo_buffer = g_serial_port1_send_vfifo_buffer;
    dma_config.send_vfifo_buffer_size = SERIAL_PORT1_SEND_BUFFER_SIZE;
    dma_config.send_vfifo_threshold_size = SERIAL_PORT1_SEND_BUFFER_SIZE / 10;

	status = hal_uart_set_dma(uart_port1, &dma_config);
    if (status != HAL_UART_STATUS_OK) {
        return SERIAL_PORT_STATUS_FAIL;
    }

    status = hal_uart_register_callback(uart_port1, serial_port1_uart_callback, NULL);
    if (status != HAL_UART_STATUS_OK) {
        return SERIAL_PORT_STATUS_FAIL;
    }

    return SERIAL_PORT_STATUS_OK;
}

void uart1_gpio_deinit(void)
{
  hal_gpio_init(HAL_GPIO_16);
  hal_pinmux_set_function(HAL_GPIO_16, HAL_GPIO_16_GPIO16); // Set the pin to operate in GPIO mode.
  hal_gpio_set_direction(HAL_GPIO_16, HAL_GPIO_DIRECTION_OUTPUT);
  hal_gpio_set_output(HAL_GPIO_16, HAL_GPIO_DATA_LOW);
  
  hal_gpio_init(HAL_GPIO_17);
  hal_pinmux_set_function(HAL_GPIO_17, HAL_GPIO_17_GPIO17); // Set the pin to operate in GPIO mode.
  hal_gpio_set_direction(HAL_GPIO_17, HAL_GPIO_DIRECTION_OUTPUT);
  hal_gpio_set_output(HAL_GPIO_17, HAL_GPIO_DATA_LOW);
}

serial_port_status_t serial_port1_uart_deinit()
{
	hal_uart_status_t uart_ret = HAL_UART_STATUS_ERROR;

    uart_ret = hal_uart_deinit(HAL_UART_1);

    printf("atci_uart_deinit() uart_ret = %d \r\n", uart_ret);
	uart1_gpio_deinit();
    return SERIAL_PORT_STATUS_OK; 
}



/**
* @brief       Task main function
* @param[in]   pvParameters: Pointer that will be used as the parameter for the task being created.
* @return      None.
*/
/*
static void vTestTask(void *pvParameters)
{
    uint32_t idx = (int)pvParameters;

    portTickType xLastExecutionTime, xDelayTime;
    xLastExecutionTime = xTaskGetTickCount();
    xDelayTime = (1 << idx) * mainCHECK_DELAY;

    while (1) {
        vTaskDelayUntil(&xLastExecutionTime, xDelayTime);
        printf("Hello World from %u at %u \r\n", idx, xTaskGetTickCount());
    }
}
*/

#ifdef FREERTOS_TEST
#include "FreeRTOS_test.h"
void vApplicationTickHook(void)
{
    vFreeRTOS_TestISR();
}
#endif /* FREERTOS_TEST */

#ifdef MTK_SYSTEM_HANG_CHECK_ENABLE
void system_wdt_occur(hal_wdt_reset_status_t mode)
{
   if(HAL_WDT_TIMEOUT_RESET == mode)
      DevFail("Watchdog timeout");
   else
      DevFail("Invalid SW watchdog");
}

/* start watchdog, timeout is 30s. Feed it at vApplicationIdleHook
   when watchdog timeout occurs, WDT_ISR occurs, then invoke assert function */
void system_start_wdt(void)
{
    hal_wdt_config_t wdt_config;
    wdt_config.mode = SYSTEM_HANG_CHECK_WDT_MODE;
    wdt_config.seconds = SYSTEM_HANG_CHECK_TIMEOUT_DURATION;
    
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
    hal_wdt_init(&wdt_config);
    hal_wdt_register_callback(system_wdt_occur);
    hal_wdt_enable(HAL_WDT_ENABLE_MAGIC);
}
#endif

#ifdef HAL_KEYPAD_FEATURE_POWERKEY
#ifdef HAL_SLEEP_MANAGER_ENABLED
void powerkey_timer_callback(void *user_data)
{
    /*unlock sleep*/
    hal_sleep_manager_release_sleeplock(powerkey_lock_sleep_handle, HAL_SLEEP_LOCK_ALL);
    /*Stop gpt timer*/
    hal_gpt_sw_stop_timer_ms(timer_handle);
    /*free gpt timer*/
    hal_gpt_sw_free_timer(timer_handle);
    power_key_timer = false;
    log_hal_info("[pwk_main]timer callback done\r\n");
}
#endif

static void keypad_user_powerkey_handler(void)
{
    hal_keypad_status_t ret;
    hal_keypad_powerkey_event_t powekey_event;
    char *string[5] = {"release", "press", "longpress", "repeat", "pmu_longpress"};

    while (1) {
        ret = hal_keypad_powerkey_get_key(&powekey_event);

        /*If an error occurs, there is no key in the buffer*/
        if (ret == HAL_KEYPAD_STATUS_ERROR) {
            //log_hal_info("[pwk_main]powerkey no key in buffer\r\n\r\n");
            break;
        }

#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (powekey_event.state == HAL_KEYPAD_KEY_PRESS || powekey_event.state == HAL_KEYPAD_KEY_LONG_PRESS) {
            if (power_key_timer == false) {
                hal_gpt_status_t ret_gpt;
                /*get timer*/
                ret_gpt = hal_gpt_sw_get_timer(&timer_handle);
                if (ret_gpt != HAL_GPT_STATUS_OK) {
                    log_hal_info("[pwk_main]get timer handle error, ret = %d, handle = 0x%x\r\n",
                                 (unsigned int)ret_gpt,
                                 (unsigned int)timer_handle);
                    return;
                }

                /*lock sleep*/
                hal_sleep_manager_acquire_sleeplock(powerkey_lock_sleep_handle, HAL_SLEEP_LOCK_ALL);
                log_hal_info("[pwk_main]start timer\r\n");
                /*start timer*/
                hal_gpt_sw_start_timer_ms(timer_handle,
                                          POWER_KEY_LOCK_SLEEP_IN_SEC * 1000, //10 sec
                                          (hal_gpt_callback_t)powerkey_timer_callback, NULL);
                power_key_timer = true;
            }
        }
#endif
        log_hal_info("[pwk_main]powerkey data:[%d], state:[%s]\r\n", (int)powekey_event.key_data, (char *)string[powekey_event.state]);
    }
}

static void hal_powerkey_example(void)
{
    bool ret_bool;
    hal_keypad_status_t ret_state;

#ifdef HAL_SLEEP_MANAGER_ENABLED
    /*get sleep handle*/
    powerkey_lock_sleep_handle = hal_sleep_manager_set_sleep_handle(powerkey_lock_sleep_name);
    if (powerkey_lock_sleep_handle == INVALID_SLEEP_HANDLE) {
        log_hal_error("[pwk_main]:get sleep handle failed\r\n");
    }
#endif

    /*Initialize powerkey*/
    ret_bool = keypad_custom_powerkey_init();
    if (ret_bool == false) {
        log_hal_error("[pwk_main]keypad_custom_init init failed\r\n");
        return;
    }

    ret_state = hal_keypad_powerkey_register_callback((hal_keypad_callback_t)keypad_user_powerkey_handler, NULL);
    if (ret_state != HAL_KEYPAD_STATUS_OK) {
        log_hal_error("[pwk_main]hal_keypad_powerkey_register_callback failed, state = %d\r\n", ret_state);
    }
}
#endif

#if 0
static void resetsys(void)
{
	hal_wdt_config_t wdt_config;
	
	wdt_config.mode = HAL_WDT_MODE_RESET;
	wdt_config.seconds = 1;
	hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
	hal_wdt_init(&wdt_config);
	hal_wdt_software_reset();
	while(1); /* waiting the system reboot */
}
#endif


int main(void)
{
    //int idx;

    /* Do system initialization, eg: hardware, clock. */
    system_init();

    /* MD init done here */

#ifdef MTK_USB_DEMO_ENABLED
	usb_boot_init();
#endif

    md_init_phase_2();

    log_init(syslog_config_save, syslog_config_load, syslog_control_blocks);

    tcpip_init(NULL, NULL);

#ifdef MTK_TCPIP_FOR_NB_MODULE_ENABLE
    nb_netif_init();
#endif

    mux_ap_init();
    ril_init();
    /*start up AP Bridge Proxy task*/
    apb_proxy_init();
#ifdef MTK_COAP_SUPPORT
    nw_coap_init();
#endif
    printf("start_conn\r\n");
    tel_conn_mgr_init();
    //tel_conn_mgr_ut_init();
    printf("end_conn\r\n");
#ifdef MTK_FOTA_ENABLE
    fota_init();
    fota_register_event(apb_proxy_fota_event_ind);
#endif
    printf("nidd init start\r\n");
    nidd_init();
    printf("nidd init finished\r\n");
#ifdef MTK_ATCI_APB_PROXY_NETWORK_ENABLE
    socket_atcmd_init_task();
    apb_upsm_init_task();
    lwm2m_atcmd_init_task();
    onenet_at_init();
#endif

    printf("start simat_proxy_tast_init\r\n");
    simat_proxy_init();
    //test_wrong_simat_command_from_AP();
    printf("end simat_proxy_tast_init\r\n");

    /* Start the scheduler. */
    SysInitStatus_Set();
#ifdef HAL_TIME_CHECK_ENABLED
    hal_time_check_enable();
#endif
#ifdef HAL_RTC_FEATURE_SW_TIMER
    rtc_sw_timer_isr();
#endif
    auto_register_init();

#ifdef MTK_GNSS_ENABLE
    gnss_demo_main();
#endif
  //  auto_register_init();

    /* 2625 lwm2m app start */
    nb_app_enter();
#ifdef MTK_LWM2M_CT_SUPPORT
    ctiot_lwm2m_client_init();
#endif

#ifdef SENSOR_DEMO
    printf("sensor_manager_init\r\n");
    sensor_manager_init();
#endif

#ifdef HAL_KEYPAD_FEATURE_POWERKEY
    hal_powerkey_example();
#endif

#ifdef __RS_FOTA_SUPPORT__
    printf("start  rs_sdk_start\r\n");
    rs_sdk_start(); 
#endif

/* wdt is enabled in bootloader, if infinite loop occurs before this line,
   wdt reset will occurs. re-config wdt here for system hang check */
#ifdef MTK_SYSTEM_HANG_CHECK_ENABLE
    //enable wdt
    system_start_wdt();
#else
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
#endif


	//set default boarterate , com port ,etc, only once
	if(0 == rin_read_defaultset_num())
	{
		serial_port_setting_uart_t uart_setting;
		ril_power_saving_mode_setting_req_t wakeup={1,NULL,NULL,NULL,NULL};
		
		ril_request_power_saving_mode_setting(RIL_EXECUTE_MODE,&wakeup,NULL,NULL);//AT+CPSMS=1

		serial_port_config_write_dev_number("connl",  SERIAL_PORT_DEV_UART_0); // AT uart0
		serial_port_config_write_dev_number("connl2", SERIAL_PORT_DEV_UART_3); // AT uart1
		serial_port_config_write_dev_number("emmi", SERIAL_PORT_DEV_USB_COM1); // gki log 
		serial_port_config_write_dev_number("uls",  SERIAL_PORT_DEV_USB_COM2); // uls

		uart_setting.baudrate = HAL_UART_BAUDRATE_9600;
		serial_port_config_write_dev_setting(SERIAL_PORT_DEV_UART_0, (serial_port_dev_setting_t *)&uart_setting);
		serial_port_config_write_dev_setting(SERIAL_PORT_DEV_UART_1, (serial_port_dev_setting_t *)&uart_setting);

		rin_write_defaultset_num(100);
	}
	
	
#ifdef MTK_N301_PRJ_NAME
	gpio_application();

	serial_port2_uart_init();
#endif

#ifdef MTK_N102_PRJ_NAME
	//netlight led
	hal_gpio_init(HAL_GPIO_20);
	hal_pinmux_set_function(HAL_GPIO_20, HAL_GPIO_20_GPIO20); // Set the pin to operate in GPIO mode.
	hal_gpio_set_direction(HAL_GPIO_20, HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_set_output(HAL_GPIO_20, HAL_GPIO_DATA_HIGH);
#endif

#ifdef LLLWM2M_DEMO_SUPPORT
	LLdevice_task_init();
#endif


	HLWatch_task_init();
    HLCopy_task_init();
    HLLink_task_init();
    HLSend_task_init();
    HLRecv_task_init();
    HLQuery_task_init();

	//serial_port1_uart_init();

    vTaskStartScheduler();
    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for (;;);
}

/* big_assert can be removed when every trace is coming in exception*/

void big_assert(void)
{
	for(uint8_t loopa = 0; loopa < 10; loopa++)
  {
  FrHslString (0x000A, "Assert! Assert! Assert! Assert! Assert! Assert! Assert! Assert!");
  }
FrHslString (0x000A, "Final line!");
}
