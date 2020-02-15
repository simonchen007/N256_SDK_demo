#ifndef _STREAM_LZMA_H
#define _STREAM_LZMA_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lzma.h"

extern void *stream_init_lzma_decoder(ML_PEER_FILE fp);
extern int stream_lzma_read(void *stream, ML_U8* buf, int len); 
extern void stream_lzma_end(void *strm);

#endif   // _STREAM_LZMA_H