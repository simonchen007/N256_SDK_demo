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

//pan socket
int tcp_pan_socket_client;
//uint8_t tcp_gps_socket_conn_flag=0;
uint8_t tcp_pan_scoket_disconnect_flag = 0;

TaskHandle_t PanxScoketHandle = NULL;


//tcp socket
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

int tcp_socket_send_all(int tcp_scoket,char *data, int length)
{
	printf("\r\n%s ,length=%d\r\n",__func__,length);
    int written_len = 0;
    while (written_len < length) {
        int ret = send(tcp_scoket, data + written_len, length - written_len, 0);
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

int tcp_socket_conn(int *tcp_socket, char *ip,char *port)
{
    struct addrinfo hints, *addr_list, *cur;
	struct sockaddr_in addr;
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
		*tcp_socket = lwip_socket(AF_INET, SOCK_STREAM, 0);
		if (*tcp_socket < 0) {
			printf("*tcp_socket create failed\r\n");
			ret = -1;
		}
		printf("---------debug 2\r\n");
		lwip_setsockopt(*tcp_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
		/* Connect */
		ret = lwip_connect(*tcp_socket, (struct sockaddr *)&addr, sizeof(addr));
		if (ret < 0) {
			lwip_close(*tcp_socket);
			printf("*tcp_socket client connect failed\r\n");
			ret = -1;
		}
		
	}else{
		memset( &hints, 0, sizeof( hints ) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		
		if ( getaddrinfo( ip, port, &hints, &addr_list ) != 0 ) {
			printf("getaddrinfo != 0, return HTTPCLIENT_UNRESOLVED_DNS\r\n");
			return -2;
		}

		/* Try the sockaddrs until a connection succeeds */
		ret = -2;
		for ( cur = addr_list; cur != NULL; cur = cur->ai_next ) {
			*tcp_socket = (int) socket( cur->ai_family, cur->ai_socktype,
											cur->ai_protocol );
			if ( *tcp_socket < 0 ) {
				ret = -1;
				continue;
			}
			lwip_setsockopt(*tcp_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
			if ( connect( *tcp_socket, cur->ai_addr, (int)cur->ai_addrlen ) == 0 ) {
				ret = 0;
				break;
			}

			close( *tcp_socket );
			ret = -1;
		}

		freeaddrinfo( addr_list );
	}
    return ret;
}

/**
  * @brief  Tcp client create socket, connect tcp server and send/receive data.
  * @param  None
  * @return int: test result of tcp_client
  */
int ll_tcp_socket_receive(void *not_used)
{
	printf("%s\r\n",__func__);
	fd_set readset;
    int ret;
   // struct sockaddr_in addr;
    int rlen =0,rcv_len =0;
    char rcv_buf[1000] = {0};
	//char result_buf[1024] = {0};
	uint32_t max_len = sizeof(rcv_buf);
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
		
    printf("tcp_client_ip_test starts\r\n");
	while(1)
	{
		printf("\r\n-------------------------s1=%d\r\n",rcv_len);
		FD_ZERO(&readset);
		FD_SET(tcp_pan_socket_client, &readset);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		
		if(tcp_pan_scoket_disconnect_flag == 1)
		{
			tcp_pan_scoket_disconnect_flag = 0;
			break;
		}
		
	   // printf("TCP client waiting for data...");
		ret = lwip_select(tcp_pan_socket_client+1, &readset, NULL, NULL, &timeout);
		if(ret>0)
		{
			if (FD_ISSET(tcp_pan_socket_client, &readset))
			{
				//memset(rcv_buf,0,sizeof(rcv_buf));
				if(rcv_len >= max_len)
					rcv_len =0;
				rlen = lwip_recv(tcp_pan_socket_client, rcv_buf + rcv_len, max_len - rcv_len, 0);
				if(rlen <= 0)
				{	printf("tcp_socket1_test rcv_len= %d, rlen = %d\r\n",rcv_len,rlen);
					break;
				}
				rcv_len = rcv_len + rlen;
				LL_send_message(MSG_ID_HLLINK_AT_TCP_RECV_ACK,NULL);
				hal_uart_send_dma(HAL_UART_0,"receive\r\n",9);
			}
		}else if(ret == -1)
		{	printf("tcp_socket1_test ret == -1\r\n");
			break;
		}
	}
	/* close */
	ret = lwip_close(tcp_pan_socket_client);
	printf("TCP client tcp_pan_socket_client close:ret = %d\r\n", ret);
	printf(" ------- net receive error 3\r\n");
	PanxScoketHandle = NULL;
	LL_send_message(MSG_ID_HLLINK_AT_TCP_CLOSE_REQ,NULL);
    vTaskDelete(NULL);
//    return ret;
//idle:
    return -1;
}

void ll_tcp_send_cmd(char send_buffer[], uint32_t send_length)
{
  
	printf("\r\nsend_buffer=%s\r\n",send_buffer);
	int ret=0;
	ret = tcp_socket_send_all(tcp_pan_socket_client, send_buffer, send_length);
	if(ret == -1)
	{
		printf(" ------- net send error 2\r\n");
		LL_send_message(MSG_ID_HLLINK_AT_TCP_CLOSE_REQ,NULL);
		//tcp_gps_socket_conn_flag = 0;
		//vTaskDelete(PanxScoketHandle);
	}else{
	}
	
}
