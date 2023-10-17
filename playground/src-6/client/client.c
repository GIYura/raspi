#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345

int main(int argc, char *argv[]) 
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;
#if 0
    struct hostent *host; /* host information */
    struct in_addr h_addr; /* Internet address */

    if ((host = gethostbyname(argv[1])) == NULL) 
    {
        fprintf(stderr, "(mini) nslookup failed on '%s'\n", argv[1]);
        exit(1);
    }
    h_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
    printf("%s\n", inet_ntoa(h_addr));
#endif

#if 1
    memset(recvBuff, '0',sizeof(recvBuff));

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if (fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    }

    if (n < 0)
    {
        printf("\n Read error \n");
    }

    close (sockfd);
#endif
    return 0;
}
