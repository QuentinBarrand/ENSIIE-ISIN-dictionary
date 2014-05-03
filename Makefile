GCC_OPTS = -O3

all: main.c Dconfig.o Ddictionary.o Dnode.o
	gcc -o dictionnaire $(GCC_OPTS) main.c Dconfig.o Dnode.o Ddictionary.o

Dconfig.o: Dconfig.h Dconfig.c
	gcc -c -o $@ $(GCC_OPTS) Dconfig.c	

Ddictionary.o: Ddictionary.h Ddictionary.c
	gcc -c -o $@ $(GCC_OPTS) Ddictionary.c


Dnode.o: Dnode.h Dnode.c
	gcc -c -o $@ $(GCC_OPTS) Dnode.c

# Build targets

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g"

clean:
	rm -rf *.o
	rm dictionnaire