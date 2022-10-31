clean: OS
	rm *.o
OS: init main.o
	gcc -o ./OS main.o
init :
main.o: main.c
	gcc -c main.c -o main.o
