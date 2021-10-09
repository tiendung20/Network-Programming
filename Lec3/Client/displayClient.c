#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>

#define MAXLINE 4096

void clean_stdin(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

void strLwr(char s[])
{
    for (int i = 0; i < strlen(s); i++)
    {
        s[i] = tolower(s[i]);
    }
}

int numAlpha(char str[])
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '\t' || str[i] == ' ')
            return 0;
    }
    return 1;
}

void signIn(int sockfd)
{
    char strsend[50], s[20];
    int check;
    do
    {
        memset(strsend, 0, sizeof(strsend));
        do
        {
            printf("Username: ");
            fgets(s, 20, stdin);
            check = numAlpha(s);
            if (s[strlen(s) - 1] == '\n')
                s[strlen(s) - 1] = '\0';
            if (strlen(s) < 1 || check == 0)
                printf("Please enter again!\n");
        } while (strlen(s) < 1 || check == 0);
        strcat(strsend, s);
        strcat(strsend, "\t");
        do
        {
            printf("Password: ");
            fgets(s, 20, stdin);
            check = numAlpha(s);
            if (s[strlen(s) - 1] == '\n')
                s[strlen(s) - 1] = '\0';
            if (strlen(s) < 1 || check == 0)
                printf("Please enter again!\n");
        } while (strlen(s) < 1 || check == 0);
        strcat(strsend, s);
        send(sockfd, strsend, strlen(strsend), 0);
        memset(s, 0, sizeof(s));
        if (recv(sockfd, s, MAXLINE, 0) == 0)
        {
            perror("The server terminated prematurely");
            exit(4);
        }
        if (atoi(s) != 1)
            printf("Incorrect username or password!\n");
    } while (atoi(s) != 1);
    printf("Access!\n");
}

void menuMain(int sockfd)
{
    char wd[15], *week_day[] = {"monday", "tuesday", "wednesday", "thursday", "friday", "all"};
    int i, c, k;
    char sendline[MAXLINE], recvline[MAXLINE];
    signIn(sockfd);
    do
    {
        send(sockfd, "sendline", strlen("sendline"), 0);
        memset(recvline, 0, sizeof(recvline));
        if (recv(sockfd, recvline, MAXLINE, 0) == 0)
        {
            perror("The server terminated prematurely");
            exit(4);
        }
        printf("-----Account: %s-----\n", recvline);
        printf("1.Read week day\n2.Logout\n3.Exit\nChoose: ");
        scanf("%d", &i);
        switch (i)
        {
        case 1:
            sprintf(sendline, "%d", 1);
            send(sockfd, sendline, strlen(sendline), 0);
            memset(recvline, 0, sizeof(recvline));
            if (recv(sockfd, recvline, MAXLINE, 0) == 0)
            {
                perror("The server terminated prematurely");
                exit(4);
            }
            if (atoi(recvline) != 1)
            {
                printf("%s", recvline);
                continue;
            }
            printf("Enter day: ");
            scanf("%s", wd);
            strLwr(wd);
            for (c = 0; c < 6; c++)
            {
                k = 0;
                if (strcmp(wd, week_day[c]) == 0)
                {
                    k = c + 2;
                    break;
                }
            }
            if (k == 0)
            {
                printf("Error: Day\n");
                sprintf(sendline, "%d", k);
                send(sockfd, sendline, strlen(sendline), 0);
                continue;
            }
            sprintf(sendline, "%d", k);
            send(sockfd, sendline, strlen(sendline), 0);
            memset(recvline, 0, sizeof(recvline));
            if (recv(sockfd, recvline, MAXLINE, 0) == 0)
            {
                perror("The server terminated prematurely");
                exit(4);
            }
            printf("%s", recvline);
            continue;
        case 2:
            sprintf(sendline, "%d", 2);
            send(sockfd, sendline, strlen(sendline), 0);
            clean_stdin();
            signIn(sockfd);
            continue;
        default:
            sprintf(sendline, "%d", 3);
            send(sockfd, sendline, strlen(sendline), 0);
            break;
        }
    } while (i < 3);
}