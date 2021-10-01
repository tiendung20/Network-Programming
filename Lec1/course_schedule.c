#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "course_schedule.h"
int classIdCheck(NodeCourse *root, Course_schedule data) {
    if(root == NULL) return 0;
    NodeCourse *str = root;
    while(str != NULL) {
        if(strcmp(str->data.classId,data.classId) == 0) return 1;
        str = str->next;
    }
    return 0;
}

void insertCourse(NodeCourse **root, Course_schedule data) {
    NodeCourse *temp = (NodeCourse*)malloc(sizeof(NodeCourse));
    temp->data = data;
    int c;
    if(*root == NULL) {
        temp->next = NULL;
        *root = temp;
        return;
    }
    else {
        c = classIdCheck(*root,data);
        if(c==1) {
            printf("ClassId already exists\n");
            return;
        }
        else {
            temp->next = *root;
            *root = temp;
        }
    }
}

void readCourse(NodeCourse **root,int *check) {
    FILE *fp = fopen("./data/course_schedule.txt","r");
    if(fp == NULL) {
        printf("Error: course_schedule.txt\n");
        *check = 1;
        return;
    }
    Course_schedule cs;
    char c,s[10];
    while(!feof(fp)) {
        fscanf(fp,"%s",cs.classId);
        fscanf(fp,"%s",cs.subjectId);
        fscanf(fp,"%c",&c);
        fscanf(fp,"%[^0123456789]s",cs.subjectName);
        fscanf(fp,"%c",&c);
        cs.week_day = c - '0';
        fscanf(fp,"%c",&c);
        cs.apm = c - '0';
        fscanf(fp,"%c",&c);
        cs.start = c - '0';
        fscanf(fp,"%c%c%c",&c,&c,&c);
        fscanf(fp,"%c",&c);
        cs.end = c - '0';
        fscanf(fp,"%c",&c);
        fscanf(fp,"%[0123456789-,]s",cs.week);
        cs.week[strlen(cs.week)-1] = '\0';
        fscanf(fp,"%[^;]s",cs.roomId);
        fgets(s,10,fp);
        insertCourse(root,cs);
    }
    fclose(fp);
}