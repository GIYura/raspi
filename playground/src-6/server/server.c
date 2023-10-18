#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define SERVER_PORT 12345
#define QUEUE_SIZE  10

int main(int argc, char *argv[])
{
    int listenFd = 0;
	int connectionFd = 0;
    struct sockaddr_in serverAddress;
    char sendBuffer[1025];
    const char* message = "Hello, I'm Raspberry Server";

    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddress, '0', sizeof(serverAddress));
    memset(sendBuffer, '0', sizeof(sendBuffer));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("192.168.0.100");
    serverAddress.sin_port = htons(SERVER_PORT);

    printf("Server IP address: %s\n", inet_ntoa(serverAddress.sin_addr));
    printf("Server port: %d\n", (int) ntohs(serverAddress.sin_port));

    bind(listenFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(listenFd, QUEUE_SIZE);

    while(1)
	{
		connectionFd = accept(listenFd, (struct sockaddr*)NULL, NULL);

		snprintf(sendBuffer, sizeof(sendBuffer), "%s\r\n", message);

		write(connectionFd, sendBuffer, strlen(sendBuffer));

        close(connectionFd);

        sleep(1);
     }
}

