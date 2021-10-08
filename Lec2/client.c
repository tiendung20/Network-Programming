#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXLINE 4096
#define SERV_PORT 3000

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    if (argc != 2)
    {
        perror("Usage: TCPClient <IP address of the server>");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(3);
    }

    do
    {
        memset(recvline, 0, sizeof(recvline));
        printf("Enter day: ");
        fgets(sendline, MAXLINE, stdin);

        if (sendline[strlen(sendline)] == '\n')
            sendline[strlen(sendline)] = '\0';
        send(sockfd, sendline, strlen(sendline), 0);

        if (recv(sockfd, recvline, MAXLINE, 0) == 0)
        {
            perror("The server terminated prematurely");
            exit(4);
        }

        printf("%s", "String received from the server: ");
        fputs(recvline, stdout);
    } while (strlen(sendline) > 0);

    exit(0);
}