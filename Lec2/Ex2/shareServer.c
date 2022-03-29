#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libgen.h>

#define MAXLINE 4096
#define SERV_PORT 3000
#define LISTENQ 8

int main(int argc, char **argv)
{
    int listenfd, connfd, n;
    // pid_t childpid;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    // creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;)
    {

        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        printf("%s\n", "Received request...");
        printf("%s\t%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        memset(buf, 0, sizeof(buf));
        if (recv(connfd, buf, MAXLINE, 0) == 0)
        {
            // error: server terminated prematurely
            perror("The server terminated prematurely");
            exit(4);
        }
        send(connfd, buf, strlen(buf), 0);
        char *filename = basename(buf);
        FILE *fp = fopen(filename, "wb");
        if (fp == NULL)
        {
            printf("ERROR\n");
            return 0;
        }
        n = 1;
        while (n > 0)
        {
            memset(buf, 0, sizeof(buf));
            n = recv(connfd, buf, MAXLINE, 0);
            if (n < 0)
                break;
            fwrite(buf, sizeof(char), 1, fp);
            send(connfd, "buf", strlen("buf"), 0);
        }

        if (n < 0)
        {
            perror("Read error");
            exit(1);
        }
        fclose(fp);
        close(connfd);
    }
    // close listening socket
    close(listenfd);

    return 0;
}