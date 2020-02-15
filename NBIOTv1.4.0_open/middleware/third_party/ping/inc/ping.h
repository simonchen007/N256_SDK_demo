
#ifndef __PING_H__
#define __PING_H__


#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif

#define PING_IP_ADDR_V4      0
#define PING_IP_ADDR_V6      1

typedef struct _ping_result
{
    uint32_t min_time;
    uint32_t max_time;
    uint32_t avg_time;
    uint32_t total_num;
    uint32_t lost_num;
    uint32_t recv_num;
} ping_result_t;

typedef struct {
    bool is_timeout;         /*When it is true, other data is invalid.*/
    uint32_t rtt;            /*The unit is ms.*/
    uint32_t ttl;            /*The TTL value in ping response packet.*/
    uint32_t packet_size;    /*The unit is byte.*/
    bool is_ipv4;            /*ipv4 is true, ipv6 is false.*/
    uint16_t ip_address[8];  /*The address has been translated by ping thread.*/
} ping_packet_result_t;

typedef enum {
    PING_PACKET_RESULT,
    PING_TOTAL_RESULT
} ping_result_type_t;

typedef void (* ping_request_result_t)(ping_result_type_t type, void* result);

typedef struct _ping_para
{
    uint32_t count;
    uint32_t size;
    uint32_t recv_timeout;
    uint32_t interval;
    ping_request_result_t callback;
    char *addr;
} ping_para_t;

typedef enum {
    PING_OK       = 0,
    PING_ERROR    = 1,
    PING_RUNNING  = 2
} ping_status_t;

//addr_type:PING_IP_ADDR_V4 or PING_IP_ADDR_V6, current only support PING_IP_ADDR_V4
ping_status_t ping_request(uint32_t count, char *addr, uint8_t addr_type, uint32_t ping_size, ping_request_result_t callback);

ping_status_t ping_request_ex(uint8_t addr_type, ping_para_t *para);

void ping_init(uint32_t count, char *addr, uint8_t addr_len, uint32_t ping_size);

uint32_t get_ping_done(void);

ping_status_t ping_close(void);

#if !PING_USE_SOCKETS
void ping_send_now(void);
#endif /* !PING_USE_SOCKETS */


#ifdef __cplusplus
}
#endif

#endif /* __PING_H__ */

