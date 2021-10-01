#ifndef _courseschedule_h_
#define _courseschedule_h_
typedef struct course_schedule
{
    char classId[10],subjectId[10],subjectName[25],week[20],roomId[10];
    int start,end,week_day,apm;
} Course_schedule;

struct nodeCourse
{
    Course_schedule data;
    struct nodeCourse *next;
};
typedef struct nodeCourse NodeCourse;

extern void readCourse(NodeCourse**,int*);
#endif