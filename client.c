#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
//special socket.h file for bsd systems:
#include <netinet/in.h>

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
	}printf("[*] created connection to host [*]\n");
	/* dude please send messages here */
	printf("[*] starting messageing... [*]\n");
	write(sock, 'y', sizeof(char));
	printf("[*] end of messages [*]\n");
	close(sock);
	return 0;
}
