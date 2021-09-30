#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student
{
    char studentId[10],password[25];
    char classId[10][10];
    int status,classNum;
} Student;

struct ListStudent
{
    Student data;
    struct ListStudent *next;
};
typedef struct ListStudent NodeS;

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
                    strcpy(temp->data.classId[temp->data.classNum-1],data.classId[0]);
                    temp->data.classNum++;
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

void readStudent(NodeS **root) {
    FILE *fp = fopen("./data/student_registration.txt","r");
    if(fp == NULL) {
        printf("Error: student_registration.txt\n");
        return;
    }
    Student st;
    while(!feof(fp)) {
        fscanf(fp,"%s %s\n",st.studentId,st.classId[0]);
        st.classNum = 1;
        st.status = 0;
        insertStudent(root,st,1);
    }
    fp = fopen("./data/User-account.txt","r");
    if(fp == NULL) {
        printf("Error: User-account.txt\n");
        return;
    }
    while(!feof(fp)) {
        fscanf(fp,"%s %s\n",st.studentId,st.password);
        insertStudent(root,st,2);
    }
}

void print(NodeS *root) {
    if(root == NULL) return;
    NodeS *str = root;
    while(str != NULL) {
        printf("%s",str->data.studentId);
        for (int i = 0; i < str->data.classNum ; i++)
        {
            printf("\t%s",str->data.classId[i]);
        }
        printf("\t%s\t%d\n",str->data.password,str->data.status);
        str = str->next;
    }
}

int main(int argc, char *argv[]) {
    NodeS *root = NULL;
    readStudent(&root);
    print(root);
    return 0;
}