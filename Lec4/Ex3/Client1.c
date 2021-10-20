#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4096
#define SERV_PORT 1125

int main()
{
    int sockfd, n, from_len;
    struct sockaddr_in servaddr, from_socket;
    socklen_t addrlen = sizeof(from_socket);
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1)
    {
        printf("Message sent: ");
        if (fgets(sendline, MAXLINE, stdin) == NULL)
            break;
        if (sendline[strlen(sendline) - 1] == '\n')
        {
            sendline[strlen(sendline) - 1] = '\0';
        }
        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        printf("Waiting...\n");
        bzero(recvline, sizeof(recvline));
        n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr *)&from_socket, &addrlen);
        recvline[n] = 0;
        printf("Message received: %s\n", recvline);
    }

    return 0;
}