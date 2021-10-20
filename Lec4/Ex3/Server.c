#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 4096

int main(int argc, char **argv)
{
    int sockfd, n, SERV_PORT, i;
    socklen_t len;
    char mesg[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    struct sockaddr_in addrClient[2];

    if (argc != 2)
    {
        perror("Usage: UDPServer <Port>");
        return 0;
    }

    SERV_PORT = (atoi(argv[1]) > 0) ? atoi(argv[1]) : 1255;

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
    i = 0;
    printf("Waiting for 2 clients ...\n");
    do
    {
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        if (i > 0)
        {
            if (addrClient[i - 1].sin_port == cliaddr.sin_port && addrClient[i - 1].sin_addr.s_addr == cliaddr.sin_addr.s_addr)
            {
                continue;
            }
        }
        addrClient[i] = cliaddr;
        i++;
    } while (i < 2);

    for (i = 0; i < 2; i++)
    {
        sendto(sockfd, mesg, n, 0, (struct sockaddr *)&addrClient[0], len);
    }

    for (;;)
    {
        len = sizeof(cliaddr);
        printf("Receiving data ...\n");

        bzero(mesg, sizeof(mesg));
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);

        for (i = 0; i < 2; i++)
        {
            if (cliaddr.sin_addr.s_addr == addrClient[i].sin_addr.s_addr && cliaddr.sin_port == addrClient[i].sin_port)
            {
                if (i == 0)
                    cliaddr = addrClient[1];
                else if (i == 1)
                    cliaddr = addrClient[0];
                break;
            }
        }
        sendto(sockfd, mesg, n, 0, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}