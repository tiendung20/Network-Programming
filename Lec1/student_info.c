#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student_info.h"

NodeS *stIdCheck(NodeS *root, Student data, int *check) {
    *check = 0;
    if(root == NULL) return NULL;
    NodeS *str = root;
    while(str != NULL) {
        if(strcmp(str->data.studentId,data.studentId) == 0) {
            *check = 1;
            return str;
        }
        str = str->next;
    }
    return NULL;
}

void insertStudent(NodeS **root, Student data, int readCheck) {
    NodeS *temp = (NodeS*)malloc(sizeof(NodeS));
    int c;
    if(*root == NULL) {
        temp->data = data;
        temp->next = NULL;
        *root = temp;
    }
    else {
        stIdCheck(*root,data,&c);
        if(c==1) {
            temp = stIdCheck(*root,data,&c);
            if(readCheck == 1) {
                for(int i=0; i<temp->data.classNum;i++) {
                    if(strcmp(temp->data.classId[i],data.classId[0]) == 0) c = -1;
                }
                if(c != (-1)) {
                    temp->data.classNum++;
                    strcpy(temp->data.classId[temp->data.classNum-1],data.classId[0]);
                }
            }
            else if(readCheck == 2) {
                strcpy(temp->data.password,data.password);
                temp->data.status = 1;
            }
        }
        else {
            temp->data = data;
            temp->next = *root;
            *root = temp;
        }
    }
}

void readStudent(NodeS **root, int *check) {
    FILE *fp = fopen("./data/student_registration.txt","r");
    if(fp == NULL) {
        printf("Error: student_registration.txt\n");
        *check = 1;
        return;
    }
    Student st;
    while(!feof(fp)) {
        fscanf(fp,"%s %s\n",st.studentId,st.classId[0]);
        st.classNum = 1;
        st.status = 1;
        insertStudent(root,st,1);
    }
    fclose(fp);
    fp = fopen("./data/User-account.txt","r");
    if(fp == NULL) {
        printf("Error: User-account.txt\n");
        *check = 1;
        return;
    }
    while(!feof(fp)) {
        fscanf(fp,"%s %s\n",st.studentId,st.password);
        st.status = 0;
        insertStudent(root,st,2);
    }
    fclose(fp);
}