

CFLAGS = -Wall -g -std=c11 -D _GNU_SOURCE

all: build
build: main.o instructorList.o receive.o send.o input.o display.o listbuffer.o shutdown.o condutils.o
	gcc $(CFLAGS) $^ -o s-talk -lpthread

%.o: %.c
	gcc -c $(CFLAGS) $< -lpthread

run: build
	./s-talk

valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all -s ./s-talk 6969 localhost 6968

clean:
	rm -f s-talk
	rm -f *.o