all: build clean

build: main.o Functions.o InputOutputFunctions.o
	gcc main.o Functions.o InputOutputFunctions.o -o cw

main.o: main.c
	gcc -c main.c

Functions.o: functions/Functions.c
	gcc -c functions/Functions.c

InputOutputFunctions.o: functions/InputOutputFunctions.c
	gcc -c functions/InputOutputFunctions.c

clean:
	rm -rf *.o
