#ifndef _ADUPS_CONFIG_H
#define _ADUPS_CONFIG_H

#include "adups_typedef.h"
#include "memory_map.h"
#ifdef __cplusplus
extern "C" {
#endif

//download
#define ABUP_DOWNLOAD_MAX_RETRY		5
#define ABUP_MAIN_DOWNLOAD_TIMER	40000
#define ABUP_UDP_MAX_BLOCK_SIZE		512    //64 128 512 1024 2048
#define ABUP_MAX_PACKET_SIZE 		1400
//device

#define ABUP_MANUFACTURER      		"n256"   			//OEM
#define ABUP_MODEL_NUMBER      		"n256"			//…Ë±∏–Õ∫≈
#define ABUP_SERIAL_NUMBER     		"345000123"

#define ABUP_PRODUCT_ID 			"1529140771"
#define ABUP_PRODUCT_SEC 			"554889c177754283bcdcf54e5e8df91b"
#define ABUP_DEVICE_TYPE 			"bicycle"
#define ABUP_PLATFORM 				"MT2625"

#define ABUP_SDK_VERSION 			"IOT4.0_R41388"
#define ABUP_APK_VERSION 			"ADUPS_V3.0"


#define ABUP_FIRMWARE_VERSION  		"1.0"
#define ABUP_HW_VERSION      		"HW01"
#define ABUP_SW_VERSION    	 		"SW01"

#define ABUP_NETWORK_TYPE			"NB"
#define ABUP_MEMORY_TOTAL			FOTA_RESERVED_LENGTH


//connection
#define ABUP_SOC_LIFE_TIME			2000
#define ABUP_SOC_LOCAL_PORT			56830
#define ABUP_SERVER_HOST			"iotlwm2m.abupdate.com:5683"  //"103.40.232.190:5683"  //"iot-lwm2m-test.abupdate.com:5683"	  //103.40.232.187	//172.17.2.254	 172.17.1.215


extern adups_int32 abup_get_main_download_ticks(void);
extern adups_int32 abup_get_download_max_retry(void);
extern adups_int32 abup_get_udp_block_size(void);
extern adups_uint8* abup_get_local_port(void);
extern adups_uint8* abup_get_server_host_and_port(void);
extern adups_uint8* abup_get_device_info_string(adups_uint8 instance,adups_uint16 res_id);
extern adups_int64 abup_get_device_info_int(adups_uint8 instance,adups_uint16 res_id);



#ifdef __cplusplus
}
#endif

#endif
