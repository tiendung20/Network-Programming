#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4096
#define SERV_PORT 3000

int main(int argc, char **argv)
{
    int sockfd, i;
    char *s = (char *)malloc(sizeof(char));
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    // basic check of the arguments
    // additional checks can be inserted
    if (argc != 3)
    {
        perror("Usage: TCPClient <IP address of the server> Direct link");
        exit(1);
    }

    // Create a socket for the client
    // If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    // Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT); // convert to big-endian order

    // Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(3);
    }
    FILE *fp = fopen(argv[2], "rb");
    if (fp == NULL)
    {
        printf("ERROR\n");
        exit(0);
    }
    send(sockfd, argv[2], strlen(argv[2]), 0);
    if (recv(sockfd, recvline, MAXLINE, 0) == 0)
    {
        // error: server terminated prematurely
        perror("The server terminated prematurely");
        exit(4);
    }
    while (!feof(fp))
    {
        i = fread(s, sizeof(char), 1, fp);
        if (i == 0)
            break;
        memset(sendline, 0, sizeof(sendline));
        strcpy(sendline, s);
        send(sockfd, sendline, sizeof(char), 0);

        if (recv(sockfd, recvline, MAXLINE, 0) == 0)
        {
            // error: server terminated prematurely
            perror("The server terminated prematurely");
            exit(4);
        }
    }

    fclose(fp);

    exit(0);
}