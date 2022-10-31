CC=gcc
CFLAGS=-pthread
DEPS = 
OBJ = main.o

clean: OS
	rm *.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

OS: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

