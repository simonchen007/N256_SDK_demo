#ifndef _ADUPS_SOCKET_H_
#define _ADUPS_SOCKET_H_
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "connection.h"
#include "internals.h"

int abup_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);
int abup_recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
int abup_sendto(int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
int abup_getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res);
int abup_socket(int domain, int type, int protocol);
int abup_connect(int s, const struct sockaddr *name, socklen_t namelen);
int abup_close(int s);
int abup_bind(int s, const struct sockaddr *name, socklen_t namelen);
void abup_freeaddrinfo(struct addrinfo *ai);



#endif
