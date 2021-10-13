#ifndef _studentinfo_h_
#define _studentinfo_h_
typedef struct Student
{
    char studentId[10], password[25];
    char classId[10][10];
    int status, classNum;
} Student;

struct ListStudent
{
    Student data;
    struct ListStudent *next;
};
typedef struct ListStudent NodeS;

extern void readStudent(NodeS **, int *);
#endif