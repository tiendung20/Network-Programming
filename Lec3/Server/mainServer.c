#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "student_info.h"
#include "course_schedule.h"
#include "session.h"

#define SERV_PORT 3000
#define LISTENQ 8

int main(int argc, char **argv)
{
    NodeS *root = NULL;
    NodeCourse *rootC = NULL;
    int check = 0;
    int listenfd, connfd;
    // pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    readStudent(&root, &check);
    readCourse(&rootC, &check);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

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
        printf("Client's IP: %-20s - Port: %d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        sessionSt(root, rootC, connfd);

        close(connfd);
    }

    close(listenfd);
}