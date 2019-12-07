#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
//#include <linux/in.h>
#include <netinet/in.h>

typedef struct
{
	int sock;
	struct sockaddr address;
	int addr_len;
} connection_t;

void* process(void* ptr)
{
	char buffer;
	connection_t* conn;
	if (!ptr) pthread_exit(0);
	conn = (connection_t*) ptr;
	read(conn->sock, &buffer, sizeof(char));
	printf("[*] The Message: (%c) [*]\n", buffer);

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
		printf("[*] ruthleth input absorbed... [*]\n");
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
	}printf("[*] socket successfully created [*]\n");
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
	printf("[*] server: ...\n");
	printf("	   listening started... [*]\n");
	while (1)
	{
		connection = (connection_t*)malloc(sizeof(connection_t));
		connection->sock = accept(sock, &connection->address, &connection->addr_len);
		if (connection->sock <= 0)
		{
			printf("[*] one unvalid connection lost... [*]\n");
			free(connection);
		}
		else
		{
			printf("[*] connection created for request [*]\n");
			pthread_create(&thread, 0, process, (void*)connection);
			pthread_detach(thread);
		}
		sleep(2);
	}
	return 0;
}
