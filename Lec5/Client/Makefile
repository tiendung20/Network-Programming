mainClient: mainClient.o displayClient.o
	gcc mainClient.o displayClient.o -o mainClient
mainClient.o: mainClient.c
	gcc -c -Wall mainClient.c
displayClient.o: displayClient.c
	gcc -c -Wall displayClient.c
clean:
	rm -f *.o *mainClient