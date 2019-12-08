#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
//special socket.h file for bsd systems:
#include <netinet/in.h>
#include <pthread.h>
#include "net.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	int port;
	int sock = -1;
	struct sockaddr_in address;
	struct hostent* host;
	int len;
	if (argc != 3)
	{
		printf("[*] ruthless misspelled... [*]\n");
		return -1;
	}
	if (sscanf(argv[2], "%d", &port) <= 0)
	{
		printf("[*] unappropriate parameter: port [*]\n");
		return -2;
	}
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		printf("[*] uncreateable to socket... [*]\n");
		return -3;
	}printf("[*] socket created [*]\n");
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	host = gethostbyname(argv[1]);
	if (!host)
	{
		printf("[*] host name not resolved... [*]\n");
		return -4;
	}printf("[*] host resolved with name %s [*]\n", host->h_name);
	memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	if(connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		printf("[*] connection to host is not possible... [*]\n");
		return -5;
	}printf("[*] connection created to host [*]\n");
	connection_t* connection = (connection_t*)malloc(sizeof(connection_t));
	connection->sock = sock;
	/* dude please send messages here */
		
	printf("[*] starting messageing... [*]\n");
	pthread_t threadsend;
	pthread_create(&threadsend, 0, sendmes, (void*)connection);
	pthread_t threadrecv;
	pthread_create(&threadrecv, 0, recmes, (void*)connection);
	pthread_join(threadrecv, NULL);
	pthread_join(threadsend, NULL);
	
	printf("[*] end of messages [*]\n");
	return 0;
}
