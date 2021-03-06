#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_PORT 1255
#define MAXLINE 250

int main()
{
    int sockfd, n, from_len;
    struct sockaddr_in servaddr, from_socket;
    socklen_t addrlen = sizeof(from_socket);
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("creat socket\n");

    while (fgets(sendline, MAXLINE, stdin) != NULL)
    {
        if (sendline[strlen(sendline) - 1] == '\n')
        {
            sendline[strlen(sendline) - 1] = '\0';
        }
        printf("To Server: %s\n", sendline);
        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        bzero(recvline, sizeof(recvline));
        n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr *)&from_socket, &addrlen);
        recvline[n] = 0;
        printf("From Server: %s %d %s\n", inet_ntoa(from_socket.sin_addr), htons(from_socket.sin_port), recvline);
    }
    return 0;
}