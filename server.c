#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
//#include <linux/in.h>
#include <netinet/in.h>
#include "net.h"

connection_t* clients[1398];
int clientLimit = 0;

void* process(void* ptr)
{
        if (!ptr) pthread_exit(0);
				int i;
				int len;
        connection_t* conn = (connection_t*) ptr;
				printf("connection address is %s\n" ,inet_ntoa((*conn).address.sin_addr));
        while(1)
        {
								char buffer[256] = {0};
								read (conn->sock, buffer, 256 * sizeof(char));
                if (buffer == ""){
									printf("connection closed form client\n");
									break;
								}
								for (i = 0; i < clientLimit; i ++){
										len = strlen(buffer);
										write(clients[i]->sock, buffer, len * sizeof(char));
								}
        }
        close(conn->sock);
        free(conn);
        pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	int sock = -1;
	struct sockaddr_in address;
	int port;
	connection_t* connection;
	pthread_t thread;
	if (argc != 2)
	{
		printf("[*] input error absorbed... [*]\n");
		return -1;
	}
	if (sscanf(argv[1], "%d", &port) <= 0)
	{
		printf("[*] unappropriate variable inputed: port [*]\n");
		return -2;
	}
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		printf("[*] failed to open socket... [*]\n");
		return -3;
	}
	printf("[*] socket successfully created [*]\n");
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");//it was other in org
	address.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
	{
		printf("[*] failed to bind to the port... [*]\n");
		return -4;
	}printf("[*] binded to port successfully [*]\n");
	if (listen(sock, 5) < 0)
	{
		printf("[*] failed to listen... [*]\n");
		return -5;
	}
	while (1)
	{
		printf("[*] server: ...\n");
		printf("	   listening started... [*]\n");
		connection = (connection_t*)malloc(sizeof(connection_t));
		connection->sock = accept(sock, (struct sockaddr*) &(connection->address), &connection->addr_len);
		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &((connection->address).sin_addr), ip, INET_ADDRSTRLEN);
		printf("connection established with IP : %s and PORT : %d\n",
																		ip, ntohs((connection->address).sin_port));
		if (connection->sock <= 0)
		{
			printf("[*] one unvalid connection lost... [*]\n");
			free(connection);
		}
		else
		{
			printf("[*] connection created for client [*]\n");
			if (clientLimit < 1398){
				clients[clientLimit] = connection;
				clientLimit++;
				pthread_create(&thread, 0, process, (void*)connection);
			}
			else {
				printf("maximom clients reached \n");
			}
		}
		sleep(2);
	}
	return 0;
}
