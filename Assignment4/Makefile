CFLAGS = -Wall -fpic -coverage -lm -std=c99

processArgs.o: processArgs.h processArgs.c
	gcc -c processArgs.c -g  $(CFLAGS)

smallsh: smallsh.c processsArgs.o
	gcc -o smallish smallsh.c -g processArgs.o $(CFLAGS)

clean:
	rm -f *.o *.exe

