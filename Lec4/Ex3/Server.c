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
    int sockfd, n, i;
    socklen_t len;
    char mesg[MAXLINE], mesg1[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    struct sockaddr_in addrClient;

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

    len = sizeof(cliaddr);
    n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
    n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
    memcpy(&addrClient, &cliaddr, sizeof(cliaddr));

    for (;;)
    {
        len = sizeof(cliaddr);
        printf("Receiving data ...\n");
        bzero(mesg, sizeof(mesg));
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        sendto(sockfd, mesg, n, 0, (struct sockaddr *)&addrClient, len);
        memcpy(&addrClient, &cliaddr, sizeof(cliaddr));
    }

    close(sockfd);
    return 0;
}