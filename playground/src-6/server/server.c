#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>  /* hostent struct, gethostbyname() */
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
    int listenfd = 0;
	int connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    const char* message = "Hello, I'm Raspberry Server";

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.100");//htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);

    printf("IP address is: %s\n", inet_ntoa(serv_addr.sin_addr));
    printf("port is: %d\n", (int) ntohs(serv_addr.sin_port));

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, QUEUE_SIZE);

    while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		ticks = time(NULL);
#if 0
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
#else	
		snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", message);       
#endif
		write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        sleep(1);
     }
}
