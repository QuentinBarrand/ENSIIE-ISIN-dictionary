GCC_OPTS = -O3

all: main.c Dnode.o args.o
	gcc -o dictionnaire $(GCC_OPTS) main.c Dnode.o args.o

# Application

args.o: args.h args.c
	gcc -c -o $@ $(GCC_OPTS) args.c

# Types

Dnode.o: Dnode.h Dnode.c
	gcc -c -o $@ $(GCC_OPTS) Dnode.c

# Build targets

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g"

clean:
	rm -rf *.o
	rm dictionnaire