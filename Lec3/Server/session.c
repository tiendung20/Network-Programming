#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include "session.h"
#include "student_info.h"
#include "course_schedule.h"

#define MAXLINE 4096

Student studentLogin(NodeS *root, int *check, int connfd)
{
    Student st;
    int n;
    char buf[MAXLINE];
    memset(buf, 0, sizeof(buf));
    if ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
    {
        strcpy(st.studentId, strtok(buf, "\t"));
        strcpy(st.password, strtok(NULL, "\t"));
    }
    if (n == 0)
    {
        *check = -1;
        return st;
    }
    if (n < 0)
    {
        perror("Read error");
        exit(1);
    }
    NodeS *str = root;
    while (str != NULL)
    {
        if (strcmp(str->data.studentId, st.studentId) == 0 && strcmp(str->data.password, st.password) == 0)
        {
            *check = 1;
            st = str->data;
            return st;
        }
        str = str->next;
    }
    *check = 0;
    return st;
}

int signIn(NodeS *root, Student *st, int connfd)
{
    int c;
    char c_send[5];
    do
    {
        *st = studentLogin(root, &c, connfd);
        sprintf(c_send, "%d", c);
        if (c == 0)
        {
            send(connfd, c_send, strlen(c_send), 0);
        }
    } while (c == 0);
    if (c == 1)
    {
        send(connfd, c_send, strlen(c_send), 0);
    }
    if (c == -1)
        return 0;
    else
        return 1;
}

void printWD(Student st, NodeCourse *root, int day, int connfd)
{
    NodeCourse *str;
    int c_day = 0, i, j, k, start, end;
    char *week_day[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    char buf[MAXLINE], s[MAXLINE];
    memset(buf, 0, sizeof(buf));
    if (day > 6)
    {
        sprintf(s, "%-5s |%-15s |%-15s |%-15s |%-15s |%-15s\n", "", week_day[0], week_day[1], week_day[2], week_day[3], week_day[4]);
        strcat(buf, s);
        strcat(buf, "\n");
        for (i = 0; i < 12; i++)
        {
            sprintf(s, "%-5d ", i + 1);
            strcat(buf, s);
            for (j = 0; j < 5; j++)
            {
                str = root;
                c_day = 0;
                while (str != NULL)
                {
                    if (str->data.apm == 1)
                    {
                        start = str->data.start;
                        end = str->data.end;
                    }
                    else if (str->data.apm == 2)
                    {
                        start = str->data.start + 6;
                        end = str->data.end + 6;
                    }
                    if ((i + 1) >= start && (i + 1) <= end)
                    {
                        if ((j + 2) == str->data.week_day)
                        {
                            for (k = 0; k < st.classNum; k++)
                            {
                                if (strcmp(st.classId[k], str->data.classId) == 0)
                                {
                                    sprintf(s, "|%-15s ", str->data.roomId);
                                    strcat(buf, s);
                                    c_day = 1;
                                    break;
                                }
                            }
                        }
                    }
                    str = str->next;
                }
                if (c_day == 0)
                {
                    sprintf(s, "|%-15s ", "");
                    strcat(buf, s);
                }
            }
            strcat(buf, "\n");
        }
        send(connfd, buf, strlen(buf), 0);
        return;
    }
    char *apm[] = {"Morning", "Afternoon"};
    sprintf(s, "%-10s |%-25s |%-15s |%-15s |%-11s |%-25s |%-10s\n", "Code", "Course", "Week Day", "AM/PM", "Period", "Week", "Room");
    strcat(buf, s);
    strcat(buf, "\n");
    for (i = 0; i < st.classNum; i++)
    {
        str = root;
        while (str != NULL)
        {
            if (strcmp(st.classId[i], str->data.classId) == 0)
            {
                if (day == str->data.week_day)
                {
                    sprintf(s, "%-10s |%-25s |%-15s |%-15s |%-d-%-9d |%-25s |%-10s\n", str->data.subjectId, str->data.subjectName, week_day[day - 2], apm[str->data.apm - 1], str->data.start, str->data.end, str->data.week, str->data.roomId);
                    strcat(buf, s);
                    c_day++;
                }
            }
            str = str->next;
        }
    }
    if (c_day == 0)
        strcat(buf, "Empty\n");
    send(connfd, buf, strlen(buf), 0);
}

int sameSchedule(Student st, NodeCourse *root)
{
    int i, j;
    NodeCourse *ptr, *str;
    if (st.classNum < 2)
        return 0;
    for (i = 0; i < st.classNum; i++)
    {
        ptr = root;
        while (ptr != NULL)
        {
            if (strcmp(st.classId[i], ptr->data.classId) == 0)
                break;
            ptr = ptr->next;
        }
        for (j = i + 1; j < st.classNum; j++)
        {
            str = root;
            while (str != NULL)
            {
                if (strcmp(st.classId[j], str->data.classId) == 0)
                    break;
                str = str->next;
            }
            if (ptr->data.week_day == str->data.week_day)
            {
                if (ptr->data.apm == str->data.apm)
                {
                    if ((ptr->data.start <= str->data.end && ptr->data.end >= str->data.start) || (str->data.start <= ptr->data.end && str->data.end >= ptr->data.start))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void sessionSt(NodeS *root, NodeCourse *rootC, int connfd)
{
    int t, i;
    Student st;
    char buf[MAXLINE], sendbuf[MAXLINE];
    i = signIn(root, &st, connfd);
    if (i == 0)
        return;
    t = sameSchedule(st, rootC);

    for (;;)
    {
        if ((i = recv(connfd, buf, MAXLINE, 0)) < 0)
        {
            perror("Read error");
            exit(1);
        }
        if (i == 0)
            return;
        send(connfd, st.studentId, strlen(st.studentId), 0);
        memset(buf, 0, sizeof(buf));
        if (recv(connfd, buf, MAXLINE, 0) < 0)
        {
            perror("Read error");
            exit(1);
        }
        switch (atoi(buf))
        {
        case 1:
            if (st.status == 0)
            {
                strcpy(sendbuf, "Not registered classId\n");
                send(connfd, sendbuf, strlen(sendbuf), 0);
                continue;
            }
            else if (t == 1)
            {
                strcpy(sendbuf, "The same schedule between subjects\n");
                send(connfd, sendbuf, strlen(sendbuf), 0);
                continue;
            }
            else
            {
                sprintf(sendbuf, "%d", 1);
                send(connfd, sendbuf, strlen(sendbuf), 0);
            }
            memset(buf, 0, sizeof(buf));
            if (recv(connfd, buf, MAXLINE, 0) < 0)
            {
                perror("Read error");
                exit(1);
            }
            if (atoi(buf) == 0)
                continue;
            printWD(st, rootC, atoi(buf), connfd);
            continue;
        case 2:
            i = signIn(root, &st, connfd);
            if (i == 0)
                return;
            t = sameSchedule(st, rootC);
            continue;
        }
    }
}