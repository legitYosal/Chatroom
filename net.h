#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

typedef struct
{
	int sock;
	struct sockaddr_in address;
	socklen_t addr_len;
} connection_t;

void writeString(char* str, connection_t* conn)
{
	int len = strlen(str);
	write(conn->sock, str, len * sizeof(char));
}

void* recmes(void* ptr)
{
        if (!ptr) pthread_exit(0);
        connection_t* conn = (connection_t*) ptr;
        while(1)
        {
                char buffer[256] = {0};
                read (conn->sock, buffer, 256 * sizeof(char));
								if (buffer == ""){
									printf("connection closed by server\n");
									break;
								}
                printf("(%s) [**] RECIEVED\n");
        }
        close(conn->sock);
        free(conn);
        pthread_exit(0);
}

void* sendmes(void* ptr)
{
        if (!ptr) pthread_exit(0);
        char str[256];
				int len;
        connection_t* conn = (connection_t*) ptr;
        while(1)
        {
                scanf("%[^\n]%*c", str);
								len = strlen(str);
								write(conn->sock, str, len * sizeof(char));
        }
        close(conn->sock);
        free(conn);
        pthread_exit(0);
}
