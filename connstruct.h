#ifndef CONN_HEADER
#define CONN_HEADER

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
typedef struct
{
	int sock;
	struct sockaddr_in address;
	socklen_t addr_len;
} connection_t;

#endif
