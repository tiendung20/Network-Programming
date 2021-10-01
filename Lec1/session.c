#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

void signIn(NodeS *root, Student *st) {
    int c;
    do
    {
        *st = studentLogin(root,&c);
        if(c!=1) printf("Incorrect username or password!\n");
    } while (c != 1);
    if(c == 1) printf("Access!\n");
}

void printWD(Student st,NodeCourse *root,int day) {
    NodeCourse *str;
    int c_day = 0,i,j,k,start,end;
    char *week_day[] = {"Monday","Tuesday","Wednesday","Thursday","Friday"};
    if(day > 6) {
        printf("%-5s |%-15s |%-15s |%-15s |%-15s |%-15s\n","",week_day[0],week_day[1],week_day[2],week_day[3],week_day[4]);
        printf("\n");
        for(i = 0; i < 12; i++) {
            printf("%-5d ",i+1);
            for(j=0; j<5; j++) {
                str = root;
                c_day = 0;
                while(str != NULL) {
                    if(str->data.apm == 1) {
                        start = str->data.start;
                        end = str->data.end;
                    } else if(str->data.apm == 2){
                        start = str->data.start + 6;
                        end = str->data.end + 6;
                    }
                    if((i+1) >= start && (i+1) <= end) {
                        if((j+2) == str->data.week_day) {
                            for(k=0; k<st.classNum; k++) {
                                if(strcmp(st.classId[k],str->data.classId) == 0) {
                                    printf("|%-15s ",str->data.roomId);
                                    c_day = 1;
                                    break;
                                }
                            }
                        }
                    }
                    str = str->next;
                }
                if(c_day == 0) printf("|%-15s ","");
            }
            printf("\n");
        }
        return;
    }
    char *apm[] = {"Morning","Afternoon"};
    printf("%-10s |%-25s |%-15s |%-15s |%-11s |%-25s |%-10s\n","Code","Course","Week Day","AM/PM","Period","Week","Room");
    printf("\n");
    for(i = 0; i<st.classNum; i++) {
        str = root;
        while(str != NULL) {
            if(strcmp(st.classId[i],str->data.classId) == 0) {
                if(day == str->data.week_day) {
                    printf("%-10s |%-25s |%-15s |%-15s |%-d-%-9d |%-25s |%-10s\n",str->data.subjectId,str->data.subjectName,week_day[day-2],apm[str->data.apm-1],str->data.start,str->data.end,str->data.week,str->data.roomId);
                    c_day++;
                }
            }
            str = str->next;
        }
    }
    if(c_day == 0) printf("Empty\n");
}

void strLwr(char s[]) {
    for(int i=0;i<strlen(s);i++) {
        s[i] = tolower(s[i]);
    }
}

void sessionSt(NodeS *root, NodeCourse *rootC) {
    int c,i,k;
    char wd[15],*week_day[] = {"monday","tuesday","wednesday","thursday","friday","all"};
    Student st;
    signIn(root,&st);
    do{
        printf("-----Account: %s-----\n",st.studentId);
        printf("1.Read week day\n2.Logout\n3.Exit\nChoose: ");
        scanf("%d",&i);
        switch (i)
        {
        case 1:
            printf("Enter day: ");
            scanf("%s",wd);
            strLwr(wd);
            for(c = 0; c < 6; c++) {
                k = 0;
                if(strcmp(wd,week_day[c]) == 0) {
                    k = c + 2;
                    break;
                }
            }
            if(k == 0) {
                printf("Error: Day\n");
                continue;
            }
            printWD(st,rootC,k);
            continue;
        case 2:
            signIn(root,&st);
            c = -1;
            break;
        case 3:
            c = 0;
            break;
        }
        if(c == -1) continue;
    }while(i>0&&i<3);
}