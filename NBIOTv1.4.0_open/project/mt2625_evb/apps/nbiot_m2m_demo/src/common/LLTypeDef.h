#ifndef __LLTYPEDEF_H__
#define __LLTYPEDEF_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "syslog.h"
#include "hal_uart.h"
#include "memory_attribute.h"
#include "hal_log.h"
#include "hal_gpt.h"

#include "task_def.h"
#include "hal_gpio.h"
#include "hal_flash.h"
#include "queue.h"
#include "FreeRTOSConfig.h"


#define LowVoltage  	2200
#define StartupVoltage  2300

#define NOTIFICATION_UPDATE_TIME                  (300 * 10) //100ms unit RTC 时间修改 6分钟

#define SOMKE_RECV_COUNT 10
#define SOMKE_RECV_LEN 200

#define UARTBUFCOUNT 10
#define SERIAL_PORT1_RECEIVE_BUFFER_SIZE 1024
#define SERIAL_PORT1_SEND_BUFFER_SIZE 1024

//#define KAL_TRUE 1
//#define KAL_FALSE 0

#define MSG_ID_LLDEVICE_BASE		0x9000
enum{
	MSG_ID_LLDEVICE_AT_MFRCLLCK_REQ = MSG_ID_LLDEVICE_BASE+1,
	MSG_ID_LLDEVICE_AT_MCGDEFCONT_REQ,
	MSG_ID_LLDEVICE_AT_CFUN_REQ,
	
	MSG_ID_LLDEVICE_UART_REQ,
	MSG_ID_LLDEVICE_AT_CGATT_REQ,
	MSG_ID_LLDEVICE_AT_LWM2M_REQ,
	MSG_ID_LLDEVICE_AT_LWM2M_SEND,
	MSG_ID_LLDEVICE_AT_LWM2M_PING,
	MSG_ID_LLDEVICE_AT_LWM2M_RECV,
	MSG_ID_LLDEVICE_AT_IDLE,
	MSG_ID_LLDEVICE_DEEPSLEEP,
};

#define MSG_ID_HLWATCH_BASE		0x1000
enum{
	MSG_ID_HLWATCH_AT_1_REQ = MSG_ID_HLWATCH_BASE+1,
	MSG_ID_HLWATCH_AT_2_REQ,
	MSG_ID_HLWATCH_AT_3_REQ,
	MSG_ID_HLWATCH_4_REQ,
};

#define MSG_ID_HLCOPY_BASE		0x2000
enum{
	MSG_ID_HLCOPY_AT_1_REQ = MSG_ID_HLCOPY_BASE+1,
	MSG_ID_HLCOPY_AT_2_REQ,
	MSG_ID_HLCOPY_AT_3_REQ,
	MSG_ID_HLCOPY_4_REQ,
};
#define MSG_ID_HLLINK_BASE		0x3000
enum{
	MSG_ID_HLLINK_AT_CGATT_REQ = MSG_ID_HLLINK_BASE+1,
		
	MSG_ID_HLLINK_AT_TCP_CREATE_REQ,
	MSG_ID_HLLINK_AT_TCP_SEND_REQ,
	MSG_ID_HLLINK_AT_TCP_RECV_ACK,
	MSG_ID_HLLINK_AT_TCP_CLOSE_REQ,
	
	MSG_ID_HLLINK_AT_UDP_CREATE_REQ,
	MSG_ID_HLLINK_AT_UDP_SEND_REQ,
	MSG_ID_HLLINK_AT_UDP_RECV_ACK,
	MSG_ID_HLLINK_AT_UDP_CLOSE_REQ,
	
	MSG_ID_HLLINK_AT_TO_IDLE_REQ,
	
};
#define MSG_ID_HLSEND_BASE		0x4000
enum{
	MSG_ID_HLSEND_AT_1_REQ = MSG_ID_HLSEND_BASE+1,
	MSG_ID_HLSEND_AT_2_REQ,
	MSG_ID_HLSEND_AT_3_REQ,
	MSG_ID_HLSEND_4_REQ,
};
#define MSG_ID_HLRECV_BASE		0x5000
enum{
	MSG_ID_HLRECV_AT_1_REQ = MSG_ID_HLRECV_BASE+1,
	MSG_ID_HLRECV_AT_2_REQ,
	MSG_ID_HLRECV_AT_3_REQ,
	MSG_ID_HLRECV_4_REQ,
};
#define MSG_ID_HLQUERY_BASE		0x6000
enum{
	MSG_ID_HLQUERY_AT_1_REQ = MSG_ID_HLQUERY_BASE+1,
	MSG_ID_HLQUERY_AT_2_REQ,
	MSG_ID_HLQUERY_AT_3_REQ,
	MSG_ID_HLQUERY_4_REQ,
};



typedef struct {
    unsigned char ref_count;
    unsigned char lp_reserved;
    unsigned short int msg_len;
} local_para_struct;

typedef struct {
    unsigned char *writingBuffer;
    unsigned short int u2ToWrite;
} peer_buff_struct;

typedef int32_t msg_type;

/* The Interlayer Message structure, which is exchaged between modules. */
typedef struct {
    msg_type msg_id; /* Message identifier */
    int local_para_ptr; /* local_para pointer */
	int local_len; /* local_para pointer */
} ilm_struct;

typedef struct {
    uint8_t recv_buf[100];
    char valid;
} SpiRecvBuf;

enum
{
  task_lldevice = 0,
  task_max
};

typedef enum  {
    SYSTEM_STATE_NORMAL = 0,         
    SYSTEM_STATE_LOW_BATTERY,          
    SYSTEM_STATE_CHARGING, 
    SYSTEM_STATE_AT_TEST
} System_State_e;

typedef enum {
	Lwm2m_net_disconnect = 0,
	Lwm2m_net_connecting,
	Lwm2m_net_connected,
	Lwm2m_server_connecting,
	Lwm2m_server_connected,
	Lwm2m_send,
	Lwm2m_low_power
} Lwm2m_State;

typedef enum {
	HLLink_net_disconnect = 0,
	HLLink_net_connecting,
	HLLink_net_connected,
	HLLink_tcp_created,
	HLLink_tcp_sended,
	HLLink_udp_created,
	HLLink_udp_sended,
	HLLink_toidle,
	HLLink_idleing,
	
} HLLink_State;


typedef struct {
	char ip[20];
	char port[10];
	char deviceID[50];
	char lifeTime[20];
	char pskID[50];
	char psk[50];
} lwm2m_creat;

typedef struct {
	char devSN[30];
	float rate;
	float gps_lon;
	float gps_lat;
	float altitude;
	float direction;
	int sateNum;
	int voltage;
	int state;
	int time;	
} RINLINKStruct;

typedef struct{
	char wifi_buf[1024];
	char wifi_data_valid;
} WIFI_SCAN;

#define LL_ASSERT(expr)   {if(!(expr)) { configASSERT(0); }else {}}


unsigned char LL_send_message(msg_type msg_id, int local_para_ptr);
unsigned char LL_send_message_para(msg_type msg_id, int local_para_ptr, int len);
unsigned char LL_send_message_from_isr(msg_type msg_id, int local_para_ptr, int len);

void LLdelay_ms(uint32_t ms);
void ResetSystem(void);
void print_hex(uint8_t buf[], int len);
uint8_t get_crc_sum(uint8_t buf[], int len);

//extern uint8_t serial1_buf[UARTBUFCOUNT][SERIAL_PORT1_RECEIVE_BUFFER_SIZE];
extern SpiRecvBuf spiBuf[UARTBUFCOUNT];
extern uint8_t somkeRecv[SOMKE_RECV_COUNT][SOMKE_RECV_LEN];
extern int g_smokeData_valid;
extern hal_uart_port_t uart_port1;
extern WIFI_SCAN wifi_data;
#endif /*__LLTYPEDEF_H__*/
