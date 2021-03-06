#ifndef _STREAM_INTERFACE_H
#define _STREAM_INTERFACE_H

#ifdef __cplusplus
    extern "C" {
#endif

bool stream_inf_install_stream_manager(int header_stream_type);
void* stream_inf_init_stream_decoder(ML_PEER_FILE f);
int stream_inf_stream_read(void *stream, ML_U8* buf, int len);
void stream_inf_stream_end(void *strm);

#ifdef __cplusplus
    }
#endif

#endif // _STREAM_INTERFACE_H