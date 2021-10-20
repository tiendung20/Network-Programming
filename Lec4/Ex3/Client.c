#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4096

int main(int argc, char **argv)
{
    int sockfd, n, from_len, SERV_PORT;
    struct sockaddr_in servaddr, from_socket;
    socklen_t addrlen = sizeof(from_socket);
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    if (argc != 3)
    {
        perror("Usage: UDPClient <IP address of the server> <Port>");
        return 0;
    }

    SERV_PORT = (atoi(argv[2]) > 0) ? atoi(argv[2]) : 1255;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("creat socket\n");

    sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr *)&from_socket, &addrlen);
    do
    {
        printf("Send message: ");
        fgets(sendline, MAXLINE, stdin);
        if (sendline[strlen(sendline) - 1] == '\n')
        {
            sendline[strlen(sendline) - 1] = '\0';
        }
        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        do
        {
            bzero(recvline, sizeof(recvline));
            n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr *)&from_socket, &addrlen);
        } while (servaddr.sin_addr.s_addr != from_socket.sin_addr.s_addr || servaddr.sin_port != from_socket.sin_port);
        recvline[n] = '\0';
        printf("Message received: %s\n", recvline);
    } while (strlen(recvline) < 0);

    return 0;
}