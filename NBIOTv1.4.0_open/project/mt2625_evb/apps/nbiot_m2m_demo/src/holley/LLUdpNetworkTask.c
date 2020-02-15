#include "LLTypeDef.h"
#include "task_def.h"
#include "ril.h"
#include "hal_rtc_internal.h"
#include "apb_proxy.h"
#include "hal_wdt.h"
#include "nvdm.h"
#include "app_common_header.h"

#include "task.h"
#include "sockets.h"
#include "syslog.h"
#include "netdb.h"
#include "lwip/ip_addr.h"

#ifdef LL_NET_STATE_LED_SUPPORT
#include "LLNetState.h"
#endif

int get_message_id(uint8_t *buf)
{
	int id = -1;
/*	
	if(buf[2] == SAVECAR_NET_CLOSE_ATTACHMENT)
	{
		hal_uart_send_dma(HAL_UART_0,"id=0\r\n",6);
		id = 0;
	}
*/
	return id;
}

//udp socket
int udp_socket_client;
struct sockaddr_in addr;
int udp_start_flag = 0;

int udp_socket_send_all(int udp_scoket,char *data, int length)
{
	printf("\r\n%s ,length=%d\r\n",__func__,length);
#ifdef LL_NET_STATE_LED_SUPPORT
	g_nb_net_state = NB_NET_DATA_SEND;
#endif
    int written_len = 0;
    while (written_len < length) {
        int ret = lwip_sendto(udp_scoket, data + written_len, length - written_len, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (ret > 0) {
            written_len += ret;
            continue;
        } else if (ret == 0) {
            return written_len;
        } else {
            return -1; /* Connnection error */
        }
    }

    return written_len;
}


static bool isValidIP(const char *s)
{
    int s1,s2,s3,s4;
    if (sscanf(s, "%d.%d.%d.%d", &s1, &s2, &s3, &s4) != 4)
    {
        return false;
    }
    if ((s1 & 0xffffff00) || (s2 & 0xffffff00) || (s3 & 0xffffff00) || (s4 & 0xffffff00))
    {
        return false;
    }
    else
    {
        return true;
    }
}

int udp_socket_conn(int *udp_socket, char *ip,char *port)
{
    struct addrinfo hints, *addr_list, *cur;
	static bool L_is_dns_ready = false;
    int ret = 0;
	struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
	
	printf("---------debug ip=%s\r\n",ip);
	printf("---------debug port=%s\r\n",port);
    
    if(isValidIP(ip))
	{
		printf("---------debug ip =%s port= %s ok\r\n",ip,port);
		uint16_t port1;
		port1 = atoi(port);
		memset(&addr, 0, sizeof(addr));
		addr.sin_len = sizeof(addr);
		addr.sin_family = AF_INET;
		addr.sin_port = lwip_htons(port1);
		addr.sin_addr.s_addr = inet_addr(ip); 
		//inet_addr_from_ipaddr(&addr.sin_addr, netif_ip4_addr(sta_if));
		printf("---------debug 1\r\n");
		/* Create the socket */
		*udp_socket = lwip_socket(AF_INET, SOCK_DGRAM, 0);
		if (*udp_socket < 0) {
			printf("*udp_socket create failed\r\n");
			ret = -1;
		}
		#if 0
		printf("---------debug 2\r\n");
		//lwip_setsockopt(*udp_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
		/* Connect */
		ret = lwip_connect(*udp_socket, (struct sockaddr *)&addr, sizeof(addr));
		if (ret < 0) {
			lwip_close(*udp_socket);
			printf("*udp_socket client connect failed\r\n");
			ret = -1;
		}
		#endif
	}else{
		
		if (L_is_dns_ready == false){
			ip_addr_t ns;
			ip_addr_set_ip4_u32(&ns, ipaddr_addr("218.4.4.4"));
			dns_setserver(0, &ns);
			L_is_dns_ready = true;
		}
		
		memset( &hints, 0, sizeof( hints ) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
		
		if ( getaddrinfo( ip, port, &hints, &addr_list ) != 0 ) {
			printf("-------------  getaddrinfo != 0, return HTTPCLIENT_UNRESOLVED_DNS\r\n");
			return -2;
		}
		printf("xufushuai -------------  getaddrinfo = 0, return pass\r\n");
		/* Try the sockaddrs until a connection succeeds */
		ret = -2;
		for ( cur = addr_list; cur != NULL; cur = cur->ai_next ) {
			memcpy(&addr, cur->ai_addr, sizeof(addr));
			*udp_socket = (int) socket( cur->ai_family, cur->ai_socktype,
											cur->ai_protocol );
			if ( *udp_socket < 0 ) {
				ret = -1;
				continue;
			}
			//lwip_setsockopt(*udp_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
			if ( connect( *udp_socket, cur->ai_addr, (int)cur->ai_addrlen ) == 0 ) {
				ret = 0;
				break;
			}

			close( *udp_socket );
			ret = -1;
		}

		freeaddrinfo( addr_list );
	}
    return ret;
}

int udp_socket_receive(void *not_used)
{
	fd_set readset;
	int rlen = 0;
    int ret;
    char rcv_buf[100] = {0};
    struct timeval timeout;
	int id = 0;
	uint16_t buf_count = 0;
	int server_message_flag =0;
	
	while (1) 
	{
		if(udp_start_flag == 0)
		{
			break;
		}
		printf("--- ---------- udp_client_test lwip_select\r\n");
		FD_ZERO(&readset);
		FD_SET(udp_socket_client, &readset);
		timeout.tv_sec = 1;
        timeout.tv_usec = 0;
		ret = lwip_select(udp_socket_client+1, &readset, NULL, NULL, &timeout);
		if(ret>0)
		{
			if (FD_ISSET(udp_socket_client, &readset))
			{
#ifdef LL_NET_STATE_LED_SUPPORT
				g_nb_net_state = NB_NET_DATA_RECV;
#endif
				memset(rcv_buf,0,sizeof(rcv_buf));
				rlen = lwip_read(udp_socket_client, rcv_buf, sizeof(rcv_buf) - 1);
				
				if (rlen <= 0) {
					lwip_close(udp_socket_client);
					printf("---- UDP server recv failed\r\n");
					break;
				}else{
					printf("\r\n------------- rcv_buf=%s ---------------------------\r\n",rcv_buf);
					hal_uart_send_dma(HAL_UART_0,"receive\r\n",9);
					//print_hex(rcv_buf, rlen);
					LL_send_message(MSG_ID_HLLINK_AT_UDP_RECV_ACK,NULL);
					/*
					memcpy(saveCarBuffer[buf_count],rcv_buf,rlen);
					saveCarBuffer[buf_count][rlen] = '\0';
					saveCarBufferLength[buf_count] = rlen;
					id = get_message_id(rcv_buf);
					printf("---- recv id=%d\r\n",id);
					server_message_flag = buf_count;
					buf_count ++;
					if(id == 0)
					{
						LL_send_message(SAVECAR_NET_CLOSE_ATTACHMENT,server_message_flag);
					}
					else if(id == 1)
					{
						LL_send_message(SAVECAR_NET_START_ATTACHMENT,server_message_flag);
					}
					else if(id == 2)
					{
						LL_send_message(SAVECAR_NET_SET_ALARM_VALUE,server_message_flag);	
					}
					else if(id == 3)
					{
						LL_send_message(SAVECAR_NET_SERVICE_IP_PORT,server_message_flag);
					}else if(id == 4)
					{
						LL_send_message(SAVECAR_NET_REPORT_INTERVAL,server_message_flag);
					}else if(id == 5)
					{
						LL_send_message(SAVECAR_NET_AD_THRESHOLD,server_message_flag);
					}else if(id == 6)
					{
						LL_send_message(SAVECAR_NET_HEARTBEAT_RSP_FRAME,server_message_flag);
					}else if(id == 7)
					{
						LL_send_message(SAVECAR_NET_LAT_LON_FRAME,server_message_flag);
					}else{
						buf_count --;
					}
					if(buf_count == BUFFER_COUNTER_LINE)
					{
						buf_count = 0;
					}*/
					
					
				}
				
			}
		}else if(ret == -1)
		{
			break;
		}
    }

    /* Close */
    ret = lwip_close(udp_socket_client);
	udp_start_flag = 0;
	vTaskDelete(NULL);
    return -1;
}

void ll_udp_send_cmd(char send_buffer[], uint32_t send_length)
{
  
	printf("\r\nsend_buffer=%s\r\n",send_buffer);
	int ret=0;
	if(udp_start_flag == 0)
	{
		udp_start_flag = 1;
		udp_socket_conn(&udp_socket_client, "39.105.131.183", "9920");
		if (pdPASS != xTaskCreate(udp_socket_receive,
								  "socket",
								  1024*4 / sizeof(portSTACK_TYPE),
								  NULL,
								  1/*SOCKET_EXAMPLE_TASK_PRIO*/,
								  NULL)) {
			printf("Cannot create socket_sample_task\r\n");
		}
	}
	ret = udp_socket_send_all(udp_socket_client, send_buffer, send_length);
	if(ret == -1)
	{
		printf(" ------- net send error 2\r\n");
		//LL_send_message(MSG_ID_LLDEVICE_udp_DISCONNECT,NULL);
		//udp_gps_socket_conn_flag = 0;
		//vTaskDelete(GpsxScoketHandle);
	}else{
		
	}	
}

void ll_upd_disconnect()
{
	udp_start_flag = 0;
}

int ll_udp_get_state()
{
	return udp_start_flag;
}