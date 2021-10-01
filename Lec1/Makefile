main: main.o course_schedule.o
	gcc main.o course_schedule.o -o main
main.o: main.c
	gcc -c -Wall main.c
course_schedule.o: course_schedule.c
	gcc -c -Wall course_schedule.c
clean:
	rm -f *.o *main