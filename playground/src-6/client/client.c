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

int main(int argc, char* argv[]) 
{
    int socketFd = 0;
    int n = 0;
    char receiveBuffer[1024];
    struct sockaddr_in serverAddress;

    if (argc != 3)
    {
        printf("Invalid number of arguments\n");
        return 1;
    }
    
    memset(receiveBuffer, '0',sizeof(receiveBuffer));

    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serverAddress, '0', sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &serverAddress.sin_addr) <= 0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if (connect(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    while ((n = read(socketFd, receiveBuffer, sizeof(receiveBuffer)-1)) > 0)
    {
        receiveBuffer[n] = 0;
        if (fputs(receiveBuffer, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    }

    if (n < 0)
    {
        printf("\n Read error \n");
    }

    close (socketFd);

    return 0;
}

