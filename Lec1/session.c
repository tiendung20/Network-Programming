#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "session.h"
#include "student_info.h"
#include "course_schedule.h"

Student studentLogin(NodeS *root, int *check) {
    Student st;
    NodeS *str = root;
    printf("Username: ");
    scanf("%s",st.studentId);
    printf("Password: ");
    scanf("%s",st.password);
    while(str != NULL) {
        if(strcmp(str->data.studentId,st.studentId) == 0 && strcmp(str->data.password,st.password) == 0) {
            *check = 1;
            st = str->data;
            return st;
        }
        str = str->next;
    }
    *check = 0;
    return st;
}

void printWD(Student st,NodeCourse *root,int day) {
    NodeCourse *str;
    char week_day[15];
    char apm[15];
    int c_day = 0,i;
    switch (day)
    {
    case 2:
        strcpy(week_day,"Monday");
        break;
    case 3:
        strcpy(week_day,"Tuesday");
        break;
    case 4:
        strcpy(week_day,"Wednesday");
        break;
    case 5:
        strcpy(week_day,"Thursday");
        break;
    case 6:
        strcpy(week_day,"Friday");
        break;
    default:
        break;
    }
    printf("%-10s %-25s %-15s %-15s %-11s %-25s %-10s\n","Code","Course","Week Day","AM/PM","Period","Week","Room");
    for(i = 0; i<st.classNum; i++) {
        str = root;
        while(str != NULL) {
            if(strcmp(st.classId[i],str->data.classId) == 0) {
                if(day == str->data.week_day) {
                    if(str->data.apm == 1) strcpy(apm,"Morning");
                    else if(str->data.apm == 2) strcpy(apm,"Afternoon");
                    printf("%-10s %-25s %-15s %-15s %-d-%-9d %-25s %-10s\n",str->data.subjectId,str->data.subjectName,week_day,apm,str->data.start,str->data.end,str->data.week,str->data.roomId);
                    c_day++;
                }
            }
            str = str->next;
        }
    }
    if(c_day == 0) printf("Empty\n");
}

void sessionSt(NodeS *root, NodeCourse *rootC) {
    int c,i;
    int wd;
    Student st;
    do
    {
        st = studentLogin(root,&c);
        if(c!=1) printf("Incorrect username or password!\n");
    } while (c != 1); 
    if(c == 1) printf("Access!\n");
    do{
        printf("-----Account: %s-----\n",st.studentId);
        printf("1.Read week day\n2.Read week schedule\n3.Logout\n4.Exit\nChoose: ");
        scanf("%d",&i);
        switch (i)
        {
        case 1:
            printf("Enter day: ");
            scanf("%d",&wd);
            printWD(st,rootC,wd);
            continue;
        }
    }while(i>0&&i<3);
}