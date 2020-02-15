#ifndef _ADUPS_TIMER_H_
#define _ADUPS_TIMER_H_
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "math.h"
#include "adups_typedef.h"
#include "abup_stdlib.h"
#include "FreeRTOS.h"
#include "timers.h"


TimerHandle_t abup_creat_timer( const char * const pcTimerName, const uint32_t xTimerPeriodInTicks, const UBaseType_t uxAutoReload, void * const pvTimerID, TimerCallbackFunction_t pxCallbackFunction );
void abup_start_timer(TimerHandle_t timer_id);
void abup_stop_timer(TimerHandle_t timer_id);
void abup_delete_timer(TimerHandle_t timer_id);
void abup_reset_timer(TimerHandle_t timer_id,adups_uint32 delay);



#endif
