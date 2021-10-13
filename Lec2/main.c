#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "student_info.h"
#include "course_schedule.h"
#include "session.h"

#define MAXLINE 4096
#define SERV_PORT 3000

int main(int argc, char **argv)
{
    NodeS *root = NULL;
    NodeCourse *rootC = NULL;
    int check = 0;
    readStudent(&root, &check);
    readCourse(&rootC, &check);
    if (check == 1)
        return 0;
    int sockfd;
    struct sockaddr_in servaddr;

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

    sessionSt(root, rootC, sockfd);

    exit(0);
}