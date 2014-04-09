GCC_OPTS = -O3

all: main.c utils.o
	gcc -o dictionnaire $(GCC_OPTS) main.c utils.o

utils.o:
	gcc -c -o $@ $(GCC_OPTS) utils.c

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g"

clean:
	rm -rf *.o
	rm dictionnaire