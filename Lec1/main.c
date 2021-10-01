#include <stdio.h>
#include "student_info.h"
#include "course_schedule.h"
#include "session.h"

int main(int argc, char *argv[]) {
    NodeS *root = NULL;
    NodeCourse *rootC = NULL;
    readStudent(&root);
    readCourse(&rootC);
    sessionSt(root,rootC);
    return 0;
}