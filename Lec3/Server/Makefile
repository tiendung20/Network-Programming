mainServer: mainServer.o student_info.o course_schedule.o session.o
	gcc mainServer.o student_info.o course_schedule.o session.o -o mainServer
mainServer.o: mainServer.c
	gcc -c -Wall mainServer.c
student_info.o: student_info.c
	gcc -c -Wall student_info.c
course_schedule.o: course_schedule.c
	gcc -c -Wall course_schedule.c
session.o: session.c
	gcc -c -Wall session.c
clean:
	rm -f *.o *mainServer