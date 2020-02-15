#ifndef _CRC_H
#define _CRC_H
 
//#include <string.h>
//#include "md5.h"

#ifdef __cplusplus
extern "C" {
 
#endif

unsigned int calc_crc(ML_U8* pbuf, int len);
void generate_crc_table(ML_U32 *p_crc, int nsize, int frame_size, char *file, int type);
ML_BOOL generate_file_crc(ML_U32 *p_crc, ML_U32 *p_nsize, char *file, int type);

#ifdef __cplusplus
 
}
#endif
 
#endif //_CRC_H
