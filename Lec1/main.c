#include <stdio.h>
#include "student_info.h"
#include "course_schedule.h"
#include "session.h"

int main(int argc, char *argv[]) {
    NodeS *root = NULL;
    NodeCourse *rootC = NULL;
    int check = 0;
    readStudent(&root,&check);
    readCourse(&rootC,&check);
    if(check == 1) return 0;
    sessionSt(root,rootC);
    return 0;
}