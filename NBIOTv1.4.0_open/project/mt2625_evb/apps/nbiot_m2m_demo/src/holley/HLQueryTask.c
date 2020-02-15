
#include "LLTypeDef.h"
#include "task_def.h"
#include "ril.h"
#include "hal_rtc_internal.h"
#include "apb_proxy.h"
#include "hal_wdt.h"
#include "nvdm.h"
#include "app_common_header.h"
#include "hal.h"

log_create_module(HLQUERY, PRINT_LEVEL_DEBUG);

#define LOGE(fmt,arg...)        LOG_E(HLQUERY, "[HLQueryTask.c] "fmt,##arg)
#define LOGW(fmt,arg...)        LOG_W(HLQUERY, "[HLQueryTask.c] "fmt,##arg)
#define LOGI(fmt,arg...)        LOG_I(HLQUERY, "[HLQueryTask.c] "fmt,##arg)

QueueHandle_t  g_HLQuery_queue;
TaskHandle_t   g_HLQuery_task_handle;
TimerHandle_t  xTimerHLQueryTask = NULL;
hal_rtc_time_t rtc_time;
signed char  pTime_zone;

typedef char Boolean ;

#define DEBUG_PORT  HAL_UART_0



void uart_send_str_data(hal_uart_port_t uart_port, char *str_data)
{
	hal_uart_send_dma(uart_port, str_data, strlen(str_data));
}

static Boolean vgCharsToInt8 (char *charA, char *result)
{
  Boolean conversion = TRUE;
  char    *charB = charA + 1;
  char    intA = 0;
  char    intB = 0;

  if ((*charA >= '0') && (*charA <= '9'))
  {
    intA = *charA - '0';
  }
  else
  {
    conversion = FALSE;
  }

  if ((conversion == TRUE) && (*charB >= '0') && (*charB <= '9'))
  {
    intB = *charB - '0';
  }
  else
  {
    conversion = FALSE;
  }

  if (conversion == TRUE)
  {
    *result = (intA * 10) + intB;
  }
  else
  {
    *result = 0;  /* reset */
  }

  return (conversion);
}


#define CCLK_DATA_AND_TIME_LENGTH (20)

static Boolean vgTimeStringToRtcTime (char *inString, hal_rtc_time_t *rtc,signed char *pTime_zone)
{
    
      Boolean success    = TRUE;     /* TRUE if whole time string conversion OK */
    
      char    cIndex;        /* Loop counter */
      char    yearChk[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
      char    dateTime[7];
    
      if (strlen(inString) == CCLK_DATA_AND_TIME_LENGTH)
      {
        /* check for dividers */
        if (((inString[ 2] == '/') &&
             (inString[ 5] == '/') &&
             (inString[ 8] == ',') &&
             (inString[11] == ':') &&
             (inString[14] == ':') &&
            ((inString[17] == '+') || (inString[17] == '-'))))
        {
          /* convert time and date values */
          for (cIndex = 0; cIndex < 7; cIndex++ )
          {
            if (vgCharsToInt8 ((inString + (cIndex * 3)), &dateTime[cIndex]) == FALSE)
            {
              success = FALSE;
            }
          }
    
          if (success == TRUE)
          {
    
            /* date */
            rtc->rtc_year     = dateTime[0];
            rtc->rtc_mon      = dateTime[1];
            rtc->rtc_day      = dateTime[2];
            rtc->rtc_hour     = dateTime[3];
            rtc->rtc_min      = dateTime[4];
            rtc->rtc_sec      = dateTime[5];
            if(rtc->rtc_day > yearChk[rtc->rtc_mon-1])
            {
                return FALSE;
            }
            /* format */
            if(inString[17] == '+')
            {
               *pTime_zone = dateTime[6]*4;      
            }
            else
            {
               *pTime_zone =0 - dateTime[6]*4 ;
            }
          }
        }
        else /* invalid dividers */
        {
          success = FALSE;
        }
      }
      else /* string length incorrect */
      {
        success = FALSE;
      }
    
      return (success);
}

void rtc_time_init(void)
{
	char timeString[]="94/08/30,11:52:00+08";  // 6th May 1994 22:10:00 GMT+2 hours
	char result = 0;
	
	uart_send_str_data(DEBUG_PORT, "hal_rtc_init\r\n");
	if(HAL_RTC_STATUS_OK != hal_rtc_init())
	{
		uart_send_str_data(DEBUG_PORT, "hal rtc init fail\r\n");
	}

	if (vgTimeStringToRtcTime (&timeString[0], &rtc_time,&pTime_zone) == TRUE)
	{ 
	  uart_send_str_data(DEBUG_PORT, "vgTimeStringToRtcTime ok\r\n");
	  if((HAL_RTC_STATUS_OK == hal_rtc_set_time(&rtc_time))&&(HAL_RTC_STATUS_OK == hal_rtc_set_quarter_hour(pTime_zone)))
	  {
		  result = 0;
		  uart_send_str_data(DEBUG_PORT, "set rtc OK\r\n");
	  }
	  else
	  {
		  result = (-1);
		  uart_send_str_data(DEBUG_PORT, "set rtc fail\r\n");
	  }

	}
	else
	{
	  uart_send_str_data(DEBUG_PORT, "vgTimeStringToRtcTime fail\r\n");
	  result = (-1);
	}

}


void HLQueryTimer_callback(TimerHandle_t pxTimer)
{
	char temp[50]={0};
	
	if(HAL_RTC_STATUS_OK != hal_rtc_get_time(&rtc_time))
	{
		uart_send_str_data(DEBUG_PORT, "hal rtc get time fail\r\n");
	}else{
		sprintf(temp,"%d-%d-%d,%d:%d:%d\r\n",rtc_time.rtc_year,rtc_time.rtc_mon,rtc_time.rtc_day, \
			rtc_time.rtc_hour,rtc_time.rtc_min,rtc_time.rtc_sec);
		uart_send_str_data(DEBUG_PORT, temp);
	}
}


static void HLQuery_task_main(void *arg)
{
    ilm_struct current_ilm;

	rtc_time_init();

	xTimerHLQueryTask = xTimerCreate("xTimerHLQueryTask", /* Just a text name, not used by the kernel. */
		(1000 / portTICK_PERIOD_MS), /* The timer period in ticks. */
		pdTRUE, /* The timers will auto-reload themselves when they expire. */
		NULL, /* Assign each timer a unique id equal to its array index. */
		HLQueryTimer_callback /* Each timer calls the same callback when it expires. */
		);
	
	if(xTimerHLQueryTask != NULL){
		xTimerStart(xTimerHLQueryTask, 0);
	}


    while (1){
        if (xQueueReceive(g_HLQuery_queue, (void *)&current_ilm, portMAX_DELAY))
		{
			switch (current_ilm.msg_id) 
			{

				case MSG_ID_HLQUERY_AT_1_REQ: // AT+CGATT?


					break;
				
				case MSG_ID_HLQUERY_AT_2_REQ:


					break;

			}
		     
            //if (current_ilm.local_para_ptr != NULL) {
                //vPortFree(current_ilm.local_para_ptr);
           // }
        }
    }
}

void HLQuery_task_init(void)
{
	
	g_HLQuery_queue = xQueueCreate(HLQUERY_QUEUE_LENGTH, sizeof(ilm_struct));

    xTaskCreate(HLQuery_task_main,
                HLQUERY_TASK_NAME,
                HLQUERY_TASK_STACKSIZE / sizeof(portSTACK_TYPE),
                NULL,
                HLQUERY_TASK_PRIO,
                &g_HLQuery_task_handle);

	LL_ASSERT(g_HLQuery_queue != NULL);
	LL_ASSERT(g_HLQuery_task_handle != NULL);
}

