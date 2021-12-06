output: main.o
	gcc main.o -o sreg.out -O3
main.o: main.c
	gcc -c main.c
clean:
	rm *.o
	rm sreg.out
