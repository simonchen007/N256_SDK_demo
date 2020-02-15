#ifndef _PEER_TYPE_H
#define _PEER_TYPE_H

typedef unsigned int          ML_U32;
typedef int                   ML_S32;
typedef unsigned short int    ML_U16;
typedef short int             ML_S16;
typedef unsigned char         ML_U8;
typedef char                  ML_S8;

typedef short int             int16_t;
typedef unsigned short int    uint16_t;
typedef unsigned char         uint8_t;
typedef unsigned int          uint32_t;
typedef int                   int32_t;


typedef unsigned long long    uint64_t;


typedef unsigned char            bool;

#define false                 0
#define true                  1
#if !defined(WIN32)
typedef unsigned long long    uintptr_t;
#endif
#if !defined(LINUX)
typedef long long             int64_t;
#endif

typedef long off_t;
typedef unsigned char        ML_BOOL;

#define MLMAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MLMIN(a,b)    (((a) < (b)) ? (a) : (b))

#define FALSE   0
#define TRUE    1

#define ML_ALIGNSIZE( x , n )             ( ( (x)+(n)-1)  & (~((n)-1))  )
#define ML_ALIGNSIZE8( x  )               ML_ALIGNSIZE( x , 8 )
#define ML_ISALIGN(x, n)                  ( ( (ML_U32)(x) & (ML_U32)((n)-1) ) == 0)

#define ML_FILE_FLAG_INVALID  0
#define ML_FILE_FLAG_NORMAL   1
#define ML_FILE_FLAG_PUSH     2

typedef struct peer_file_stru_tag
{
    ML_U32 fd;
    ML_U16 type;
    ML_U16 flag;
	ML_S8* fname;
    ML_U32 param;
    ML_U32 base_position;
    ML_U32 position;
    ML_U32 length;
	ML_U32 errorcode;
}ML_PEER_FILE_STRU;

typedef ML_PEER_FILE_STRU* ML_PEER_FILE;

#define PEER_FILETYPE_ROM         1
#define PEER_FILETYPE_FLASH       2
#define PEER_FILETYPE_BACKUP      3
#define PEER_FILETYPE_FAT         4

#ifdef LINUX	
#define ML_INLINE __inline__
#else
#define ML_INLINE __inline
#endif

#define restrict


#if defined(PEER_HAVING_MEM_TEST)
#include "peer_mem.h"
#endif

#if defined(WIN32)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <io.h>
#include <limits.h>
#include "windows.h"

#elif defined (LINUX)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <time.h>  
#include <sys/time.h>  
#include <assert.h>
#else
#include "adups_typedef.h"
#include "adups_bl_main.h"
#include "adups_bl_flash.h"
#include "stdarg.h"
#endif


#if !defined(WIN32) 
typedef struct _SYSTEMTIME
{
	ML_U16 wYear;
	ML_U16 wMonth;
	ML_U16 wDayOfWeek;
	ML_U16 wDay;
	ML_U16 wHour;
	ML_U16 wMinute;
	ML_U16 wSecond;
	ML_U16 wMilliseconds;
}SYSTEMTIME;
#endif


#define ML_SIZE_MAX 0xffffffff


/* 按平台需要细化返回值 */
/* 当RETCODE <0 时禁止删除差分包 */
/* 当RETCODE >=0 时可以删除差分包 */
typedef enum RETCODE_FOTA_ENUM 
    {
    	RETCODE_FOTA_WRITE_ERR = -2,
        RETCODE_FOTA_UPDATE_ERR = -1,
		RETCODE_FOTA_OK = 0,
	    RETCODE_FOTA_PROCESS_ERR   = 1,
		RETCODE_FOTA_PROCESS_ERRSYSTEM   = 2,
		RETCODE_FOTA_PROCESS_ERRFORMAT   = 3,
		RETCODE_FOTA_PROCESS_ERRMEM   = 4, 
		RETCODE_FOTA_PROCESS_ERRHEADER  = 5, 
	    RETCODE_FOTA_PROCESS_ERRDATA   = 6,
	    RETCODE_FOTA_PROCESS_ERRFILESPACE  = 7,
	    RETCODE_FOTA_PROCESS_PATCHED  = 8
}RETCODE_FOTA;


#endif // _PEER_TYPE_H



