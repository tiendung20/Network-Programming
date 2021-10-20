#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 4096
#define SERV_PORT 1125

int main()
{
    int sockfd, n, i = 0;
    socklen_t len;
    char mesg[MAXLINE];
    struct sockaddr_in servaddr, cliaddr, addrClient, temp;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) >= 0)
    {
        printf("Server is running at port %d\n", SERV_PORT);
    }
    else
    {
        perror("bind failed");
        return 0;
    }

    do
    {
        memcpy(&addrClient, &cliaddr, sizeof(cliaddr));
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        if (cliaddr.sin_addr.s_addr != addrClient.sin_addr.s_addr || cliaddr.sin_port != addrClient.sin_port)
        {
            i++;
        }
    } while (i < 2);

    memcpy(&addrClient, &cliaddr, sizeof(cliaddr));

    for (;;)
    {
        len = sizeof(cliaddr);
        printf("Receiving data ...\n");
        bzero(mesg, sizeof(mesg));
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        if (cliaddr.sin_addr.s_addr != addrClient.sin_addr.s_addr || cliaddr.sin_port != addrClient.sin_port)
        {
            memcpy(&temp, &cliaddr, sizeof(cliaddr));
            sendto(sockfd, mesg, n, 0, (struct sockaddr *)&addrClient, len);
        }
        else
        {
            sendto(sockfd, mesg, n, 0, (struct sockaddr *)&temp, len);
        }
    }

    close(sockfd);
    return 0;
}