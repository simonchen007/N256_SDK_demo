//#include "LLTypeDef.h"
//#include "LLDevice_msg_id.h"
#include "task_def.h"
#include "ril.h"
#include "hal_rtc_internal.h"
#include "apb_proxy.h"
#include "hal_wdt.h"
#include "nvdm.h"
#include "app_common_header.h"
#include "td1030.h"

extern uint8_t sys_lock_handle;
extern hal_uart_port_t uart_port;
void td1030_init(void);

void gps_on()
{
	td1030_sleep_setting(0);
	receive_interval(1000);
	td1030_init();
}

void gps_off()
{
	td1030_sleep_setting(1);
}

void gps_on_active()
{
	td1030_sleep_setting(0);
	//receive_interval(1000);
	//td1030_init();
}

void gps_off_active()
{
	td1030_sleep_setting(2);
}

void gps_rest(void)
{
	gpio_1_low();
	vTaskDelay(1000 / portTICK_RATE_MS);
	gpio_1_hight();
	
	td1030_init();
}

void receive_interval(int interval)
{
	char inv_100[]="$CCINV,100,*60\r\n";
	char inv_500[]="$CCINV,500,*64\r\n";
	char inv_1s[]="$CCINV,1000,*50\r\n";
	char inv_10s[]="$CCINV,10000,*60\r\n";
	char inv_60s[]="$CCINV,60000,*67\r\n";
	//td1030_init();
	if(interval == 100)
	{
		hal_uart_send_dma(uart_port,inv_100,strlen(inv_100));
	}else if(interval == 500)
	{
		hal_uart_send_dma(uart_port,inv_500,strlen(inv_500));
	}else if(interval == 1000)
	{
		hal_uart_send_dma(uart_port,inv_1s,strlen(inv_1s));
	}else if(interval == 10000)
	{
		hal_uart_send_dma(uart_port,inv_10s,strlen(inv_10s));
	}else if(interval == 60000)
	{
		hal_uart_send_dma(uart_port,inv_60s,strlen(inv_60s));
	}
}

void td1030_sleep_setting(int sleep)
{
	unsigned char light_sleep[]={0x23,0x3E,0x02,0x04,0x01,0x00,0x01,0x08,0x1E};
	if(sleep == 0)
	{
		gpio_1_hight();
		hal_uart_send_dma(uart_port, "123", 3);
		vTaskDelay(50 / portTICK_RATE_MS);
		td1030_init();
		//serial_port2_uart_init();

		//lock system do not enter sleep
        if (sys_lock_handle == 0xFF) {
            sys_lock_handle = hal_sleep_manager_set_sleep_handle("sys_init");
        }else{
			hal_sleep_manager_acquire_sleeplock(sys_lock_handle, HAL_SLEEP_LOCK_ALL);
		}
				
	}else if(sleep == 1)
	{
		printf("----------------------------------------- sleep 1 -------------------- \r\n");
		//serial_port2_uart_deinit();
		gpio_1_low();
		//hal_uart_send_dma(uart_port, light_sleep, 9);

		//release system can enter sleep
        if (sys_lock_handle != 0xFF){
            hal_sleep_manager_release_sleeplock(sys_lock_handle, HAL_SLEEP_LOCK_ALL);
        }
	}else if(sleep == 2)
	{
		printf("----------------------------------------- sleep 2 -------------------- \r\n");
		 
		hal_uart_send_dma(uart_port, light_sleep, 9);
		//vTaskDelay(200 / portTICK_RATE_MS);
		//hal_uart_send_dma(HAL_UART_2, light_sleep, 9);
	}
}

void td1030_init(void)
{
	//int interval = 0;
	//int length_nv = 0;
	//nvdm_status_t status;
	//char buf[10]={0};
	
	//启动模式
	char start_mode[]="$CCSIR,3,0*4B\r\n";
	//接收间隔
	char INV[]="$CCINV,2000,53\r\n";
	//开启msg 语句
	char RMC[]= "$CCMSG,RMC,1,1,*05\r\n";
	char GNS[]= "$CCMSG,GNS,1,1,*03\r\n";
	char GSV[]= "$CCMSG,GSV,1,1,*1B\r\n";
	//关闭msg 语句
	char GGA[]= "$CCMSG,GGA,1,0,*19\r\n";
	char GSA[]= "$CCMSG,GSA,1,0,*0D\r\n";
	//char GSV[]= "$CCMSG,GSV,1,0,*1A\r\n";
	char GLL[]= "$CCMSG,GLL,1,0,*1F\r\n";
	char VTG[]= "$CCMSG,VTG,1,0,*1D\r\n";
	char ZDA[]= "$CCMSG,ZDA,1,0,*07\r\n";
	char DTM[]= "$CCMSG,DTM,1,0,*05\r\n";
	//char GNS[]= "$CCMSG,GNS,1,0,*02\r\n";
	char GBS[]= "$CCMSG,GBS,1,0,*0E\r\n";
	char GRS[]= "$CCMSG,GRS,1,0,*1E\r\n";
	char GST[]= "$CCMSG,GST,1,0,*18\r\n";
	char TXT[]= "$CCMSG,TXT,1,0,*00\r\n";
	
	/*
	length_nv = sizeof(buf);
	status = nvdm_read_data_item("RinlinkGps", "interval", buf, &length_nv);
    if (NVDM_STATUS_OK != status) {
       printf("\r\nfail RinlinkGps interval \r\n");
	   interval = 10000;
	}else{
	   printf("\r\n-------------------------- RinlinkGps interval = %s\r\n",interval);	
	}
	receive_interval(interval);*/
	
	hal_uart_send_dma(uart_port,start_mode,strlen(start_mode));
	vTaskDelay(5 / portTICK_RATE_MS);	
	hal_uart_send_dma(uart_port,INV,strlen(INV));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,RMC,strlen(RMC));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,GNS,strlen(GNS));
	vTaskDelay(5 / portTICK_RATE_MS);	
	hal_uart_send_dma(uart_port,GGA,strlen(GGA));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,GSA,strlen(GSA));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,GSV,strlen(GSV));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,GLL,strlen(GLL));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,VTG,strlen(VTG));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,ZDA,strlen(ZDA));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,DTM,strlen(DTM));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,GBS,strlen(GBS));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,GRS,strlen(GRS));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,GST,strlen(GST));
	vTaskDelay(5 / portTICK_RATE_MS);
	hal_uart_send_dma(uart_port,TXT,strlen(TXT));

}

extern TD1030_GNS gns_data;
extern TD1030_GSV gsv_dataGP;
extern TD1030_GSV gsv_dataBD;

int td1030_rmc_parse(TD1030_RMC *rmc_data, char rcm_buff[])
{
   int rcm_data_len = 0;
   char *p_crc = NULL;
   char *p = NULL;
   char *p1 = NULL, *p2 = NULL;
   int count = 0;
   char crc_value = 0;
   char crc_value_data[3] = {0};
   //rmc_data->IsPass = 0;
   if(rcm_buff[0] != '$')
   {
	return -1;
   }
   
   //hal_uart_send_dma(HAL_UART_0,"\r\n1\r\n",5);
   
   p_crc = strchr(rcm_buff, '*');
   if(p_crc == NULL)
   {
      return -1;
   }
   //hal_uart_send_dma(HAL_UART_0,"\r\n2\r\n",5);
   p = rcm_buff;
   p = p + 1;
   crc_value = rcm_buff[1];
   rcm_data_len = p_crc - p; 
   for(count = 1; count < rcm_data_len; count++)
   {
      crc_value = crc_value ^ p[count];
   }
   sprintf(crc_value_data, "%02x",crc_value);
   printf("crc_value_data =%s,crc=%s\r\n",crc_value_data,p_crc + 1);
   
   
   crc_value_data[0] = toupper(crc_value_data[0]);
   crc_value_data[1] = toupper(crc_value_data[1]);
   
   //hal_uart_send_dma(HAL_UART_0,crc_value_data,2);
   //hal_uart_send_dma(HAL_UART_0,p_crc + 1,2);
   
   if(strncmp(crc_value_data, p_crc + 1,2) != 0)
   {
      return -1;
   }
   //hal_uart_send_dma(HAL_UART_0,"\r\n3\r\n",5);
   printf("\r\n = \r\n");
   
   char *gns = NULL;
   gns = strstr(rcm_buff,"$GNGNS");
   if(gns != NULL)
   {
	   td1030_gns_parse(&gns_data, gns);
	  // hal_uart_send_dma(HAL_UART_0,"DataTime\r\n", 10);
   }
   
   char *gsvgp = NULL;
   gsvgp = strstr(rcm_buff,"$GPGSV");
   if(gsvgp != NULL)
   {
	   td1030_gsv_parse(&gsv_dataGP, gsvgp);
	  // hal_uart_send_dma(HAL_UART_0,"DataTime\r\n", 10);
   }
   
   char *gsvbd = NULL;
   gsvbd = strstr(rcm_buff,"$BDGSV");
   if(gsvbd != NULL)
   {
	   td1030_gsv_parse(&gsv_dataBD, gsvbd);
	  // hal_uart_send_dma(HAL_UART_0,"DataTime\r\n", 10);
   }
   
   p1 = strchr(rcm_buff, ',');
   if(p1 == NULL)
   {
   	return -1;
   }
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   //hal_uart_send_dma(HAL_UART_0,"\r\n4\r\n",5);
   if((p2 - p1 -1) != 0)
   {
   	memcpy(rmc_data->UTCTime, p1 + 1, p2 - p1 -1);
   	printf("%s\r\n",rmc_data->UTCTime);
   }
  
   if(p2 + 1 != NULL)
   {
     rmc_data->State = p2[1];
   }
   printf("%c\r\n",rmc_data->State);
   if(rmc_data->State != 'A')
   {
	return -1;
   }
   // hal_uart_send_dma(HAL_UART_0,"\r\n5\r\n",5);
   p1 = strchr(p2 + 3, ',');
   if(p1 == NULL)
   {
	return -1;
   }
   if(p1 - p2 -3 != 0)
   {
     	memcpy(rmc_data->Latitudinal,p2 + 3, p1 - p2 -3);
   	printf("%s\r\n",rmc_data->Latitudinal);
   }
   //hal_uart_send_dma(HAL_UART_0,"\r\n6\r\n",5);
   if(p1 + 1 != NULL)
   {
	rmc_data->NorthSouth = p1[1];
   }
   printf("%c\r\n",rmc_data->NorthSouth);
   
   p2 = strchr(p1 + 3, ',');
   if(p2 == NULL)
   {
      return -1;
   }
   if(p2 - p1 - 3 != 0)
   {
	memcpy(rmc_data->Longitude,p1 + 3, p2 - p1 -3);
   	printf("%s\r\n",rmc_data->Longitude);
   }
   if(p1 + 1 != NULL)
   {
	rmc_data->EastWest = p2[1];
   }
   rmc_data->IsPass = 1;
   printf("%c\r\n",rmc_data->EastWest);
   
   p1 = strchr(p2 + 3, ',');
   if(p1 == NULL)
	   return -1;
   if(p1 - p2 -3 != 0)
   {
	memcpy(rmc_data->GroundSpeed,p2 + 3, p1 - p2 -3);
   	printf("%s\r\n",rmc_data->GroundSpeed);
	//hal_uart_send_dma(HAL_UART_0,rmc_data->GroundSpeed, strlen(rmc_data->GroundSpeed));
   }
   
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
	   return -1;
   if(p2 - p1 -1 != 0)
   {
	memcpy(rmc_data->GroundGuide,p1 + 1, p2 - p1 -1);
   	printf("%s\r\n",rmc_data->GroundGuide);
	//hal_uart_send_dma(HAL_UART_0,rmc_data->GroundGuide, strlen(rmc_data->GroundGuide));
   }
   //hal_uart_send_dma(HAL_UART_0,p2, 5);
   p1 = strchr(p2 + 1, ',');
   if(p1 != NULL)
   {
	memcpy(rmc_data->DataTime,p2 + 1, p1 - p2 -1);
   	printf("%s\r\n",rmc_data->DataTime);
	//hal_uart_send_dma(HAL_UART_0,rmc_data->DataTime, strlen(rmc_data->DataTime));
   }
   
   
   return 0;
}

int td1030_gns_parse(TD1030_GNS *gns_data, char rcm_buff[])
{
   int rcm_data_len = 0;
   char *p_crc = NULL;
   char *p = NULL;
   char *p1 = NULL, *p2 = NULL;
   int count = 0;
   char crc_value = 0;
   char crc_value_data[3] = {0};
   //gns_data->IsPass = 0;
  // char rcm_buff[] = "$GNGNS,122310.22,3722.425671,N,12258.856215,W,DAAA,14,0.9,1005.543247,6.5,5.2,1023*5B\r\n";
   
   if(rcm_buff[0] != '$')
   {
	return -1;
   }
   
  // hal_uart_send_dma(HAL_UART_0,rcm_buff,strlen(rcm_buff));
   
   p_crc = strchr(rcm_buff, '*');
   if(p_crc == NULL)
   {
      return -1;
   }
  // hal_uart_send_dma(HAL_UART_0,"\r\n2\r\n",5);
   p = rcm_buff;
   p = p + 1;
   crc_value = rcm_buff[1];
   rcm_data_len = p_crc - p; 
   for(count = 1; count < rcm_data_len; count++)
   {
      crc_value = crc_value ^ p[count];
   }
   sprintf(crc_value_data, "%02x",crc_value);
   printf("crc_value_data =%s,crc=%s\r\n",crc_value_data,p_crc + 1);
   
   
   crc_value_data[0] = toupper(crc_value_data[0]);
   crc_value_data[1] = toupper(crc_value_data[1]);
   
   //hal_uart_send_dma(HAL_UART_0,crc_value_data,2);
   //hal_uart_send_dma(HAL_UART_0,p_crc + 1,2);
   
   if(strncmp(crc_value_data, p_crc + 1,2) != 0)
   {
      return -1;
   }
   
   //hal_uart_send_dma(HAL_UART_0,"\r\n3\r\n",5);
   printf("\r\n = \r\n");

   p1 = strchr(rcm_buff, ',');
   if(p1 == NULL)
   {
   	return -1;
   }
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   
   p1 = strchr(p2 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   
   p1 = strchr(p2 + 1, ',');
   if(p1 == NULL)
   {
	return -1;
   }
   
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   //7
   p1 = strchr(p2 + 1, ',');
   if(p1 == NULL)
   {
	return -1;
   }
   //8
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   if(p2 - p1 -1 != 0)
   {
	memcpy(gns_data->SatelliteNum,p1 + 1, p2 - p1 -1);
   	printf("%s\r\n",gns_data->SatelliteNum);
	//hal_uart_send_dma(HAL_UART_0,gns_data->SatelliteNum, strlen(gns_data->SatelliteNum));
   }
   //9
   p1 = strchr(p2 + 1, ',');
   if(p1 == NULL)
   {
	return -1;
   }
   //10
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   //11
   p1 = strchr(p2 + 1, ',');
   if(p1 == NULL)
   {
	return -1;
   }
   
   if(p1 - p2 -1 != 0)
   {
	memcpy(gns_data->GeoidalSurface,p2 + 1, p1 - p2 -1);
   	printf("%s\r\n",gns_data->GeoidalSurface);
	//hal_uart_send_dma(HAL_UART_0,gns_data->GeoidalSurface, strlen(gns_data->GeoidalSurface));
   }
   gns_data->IsPass = 1;
   //hal_uart_send_dma(HAL_UART_0,"\r\nGNS\r\n",7);
   return 0;
}

int td1030_gsv_parse(TD1030_GSV *gsv_data, char rcm_buff[])
{
   int rcm_data_len = 0;
   char *p_crc = NULL;
   char *p = NULL;
   char *p1 = NULL, *p2 = NULL;
   int count = 0;
   char crc_value = 0;
   char crc_value_data[3] = {0};
   //gns_data->IsPass = 0;
  // char rcm_buff[] = "$GNGNS,122310.22,3722.425671,N,12258.856215,W,DAAA,14,0.9,1005.543247,6.5,5.2,1023*5B\r\n";
   
   if(rcm_buff[0] != '$')
   {
	return -1;
   }
   
   //hal_uart_send_dma(HAL_UART_0,rcm_buff,strlen(rcm_buff));
   
   p_crc = strchr(rcm_buff, '*');
   if(p_crc == NULL)
   {
      return -1;
   }
   //hal_uart_send_dma(HAL_UART_0,"\r\n2\r\n",5);
   p = rcm_buff;
   p = p + 1;
   crc_value = rcm_buff[1];
   rcm_data_len = p_crc - p; 
   for(count = 1; count < rcm_data_len; count++)
   {
      crc_value = crc_value ^ p[count];
   }
   sprintf(crc_value_data, "%02x",crc_value);
   printf("crc_value_data =%s,crc=%s\r\n",crc_value_data,p_crc + 1);
   
   
   crc_value_data[0] = toupper(crc_value_data[0]);
   crc_value_data[1] = toupper(crc_value_data[1]);
   
   //hal_uart_send_dma(HAL_UART_0,crc_value_data,2);
   //hal_uart_send_dma(HAL_UART_0,p_crc + 1,2);
   
   if(strncmp(crc_value_data, p_crc + 1,2) != 0)
   {
      return -1;
   }
   
   //hal_uart_send_dma(HAL_UART_0,"\r\n3\r\n",5);
   printf("\r\n = \r\n");

   p1 = strchr(rcm_buff, ',');
   if(p1 == NULL)
   {
   	return -1;
   }
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   
   p1 = strchr(p2 + 1, ',');
   if(p1 == NULL)
   {
	return -1;
   }
   //hal_uart_send_dma(HAL_UART_0,p1,strlen(p1));
    //hal_uart_send_dma(HAL_UART_0,"\r\n3\r\n",5);
   //卫星个数
   gsv_data->TotalSatellite = atoi(p1+1);
   
   if(gsv_data->TotalSatellite == 0)
   {
	   gsv_data->CN = 0;
	   return 0;
   }
   
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   
   p1 = strchr(p2 + 1, ',');
   if(p1 == NULL)
   {
	return -1;
   }
   
   p2 = strchr(p1 + 1, ',');
   if(p2 == NULL)
   {
	return -1;
   }
   
   p1 = strchr(p2 + 1, ',');
   if(p1 == NULL)
   {
	return -1;
   }
  // hal_uart_send_dma(HAL_UART_0,p1,strlen(p1));
    //hal_uart_send_dma(HAL_UART_0,"\r\n4\r\n",5);
   gsv_data->CN = atoi(p1+1);
   
   gsv_data->IsPass = 1;
   //hal_uart_send_dma(HAL_UART_0,"\r\nGNS\r\n",7);
   return 0;
}
