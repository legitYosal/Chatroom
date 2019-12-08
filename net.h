#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

typedef struct
{
	int sock;
	struct sockaddr address;
	int addr_len;
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
                char buffer[512] = {0};
                read (conn->sock, buffer, 512 * sizeof(char));
                printf("%s [**] RECIEVED\n");
        }
        close(conn->sock);
        free(conn);
        pthread_exit(0);
}

void* sendmes(void* ptr)
{
        if (!ptr) pthread_exit(0);
        char str[512];
        connection_t* conn = (connection_t*) ptr;
        while(1)
        {
                scanf("%[^\n]%*c", str);
                writeString(str, conn);
        }
        close(conn->sock);
        free(conn);
        pthread_exit(0);
}
