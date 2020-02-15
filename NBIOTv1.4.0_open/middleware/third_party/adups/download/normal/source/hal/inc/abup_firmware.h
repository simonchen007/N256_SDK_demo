#ifndef _ABUP_FIREWARE_H_
#define _ABUP_FIREWARE_H_
#include "FreeRTOS.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "math.h"
#include "adups_typedef.h"
#include "abup_stdlib.h"
#include "timers.h"
#include "connection.h"


typedef struct
{
    uint8_t state;
    bool supported;
    uint8_t result;
} firmware_data_t;

typedef enum
{
	ABUP_DOWNLOAD_INIT,
	ABUP_DOWNLOAD_RU,
	ABUP_DOWNLOAD_CV,
	ABUP_DOWNLOAD_ING,
	ABUP_DOWNLOAD_END
}ABUP_DOWNLOAD_STATE;


typedef struct
{
	firmware_data_t* 		firmware_data; 
	ABUP_DOWNLOAD_STATE 	state;							//FOTA STATE	
	adups_uint8 			device_id[50];
	adups_uint8 			down_host[50];
	adups_uint8 			down_uri[200];
	connection_t * 	download_session;
	adups_uint8 		 	download_session_retry_num;
	adups_uint32 			file_size;
	adups_uint32 			delta_id;
	adups_uint8 			file_md5[33];
	adups_uint16 			gcurrudpblock;
	TimerHandle_t 			xmaindownloadTimer;
	TimerHandle_t			wakeupTimer;
	TimerHandle_t 			xdownloadTimer;
	TimerHandle_t 			xdownloadTimer1;
	adups_uint8 			retry_num; 						// overtime
	adups_uint8 			data_err_retry_num; 			// data error
	adups_uint8 			retry_rucv_num;
	adups_uint8 			send_download_result_retry_num; // download report
	ADUPS_BOOL  			send_download_result;
	adups_uint16			msg_id;
	adups_uint32 			utc_time;
	adups_uint32 			start_time;
	adups_uint32 			end_time;
	adups_uint32 			status;
	adups_uint32 			err_code;
} abup_firmware_data_t;


void abup_main_download_timer_reset();
#endif
