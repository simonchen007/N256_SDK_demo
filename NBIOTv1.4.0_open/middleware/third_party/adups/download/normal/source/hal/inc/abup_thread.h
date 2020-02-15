#ifndef _ADUPS_THREAD_
#define _ADUPS_THREAD_
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "adups_typedef.h"
#include "abup_stdlib.h"

ADUPS_BOOL adups_active_pdp();
ADUPS_BOOL adups_deactive_pdp();

#define ABUP_CURR_OP_RU	0
#define ABUP_CURR_OP_CV	1
#define ADUPS_TASK_MSGID_UPDATE			100
#define ADUPS_TASK_MSGID_UPDATE_RESULT	101
#define ABUP_FOTA_TASK_NAME      			"abup_fota"
#define ABUP_FOTA_TASK_STACKSIZE        	(1024 * 50)
#define ABUP_FOTA_TASK_PRIO             	TASK_PRIORITY_NORMAL

typedef struct {
    unsigned int  msg_id;
    void *param;
} adups_message_t;



#endif
