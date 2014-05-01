GCC_OPTS = -O3

all: main.c Dnode.o Ddictionary.o
	gcc -o dictionnaire $(GCC_OPTS) main.c Dnode.o Ddictionary.o

# Application

Ddictionary.o: Ddictionary.h Ddictionary.c
	gcc -c -o $@ $(GCC_OPTS) Ddictionary.c

# Types

Dnode.o: Dnode.h Dnode.c
	gcc -c -o $@ $(GCC_OPTS) Dnode.c

# Build targets

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g"

clean:
	rm -rf *.o
	rm dictionnaire