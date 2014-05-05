GCC_OPTS = -O3

.PHONY: doc

all: main.c Dconfig.o Ddictionary.o Dnode.o DnodeList.o
	gcc -o dictionnaire $(GCC_OPTS) main.c Dconfig.o DnodeList.o Dnode.o \
		Ddictionary.o

Dconfig.o: Dconfig.h Dconfig.c
	gcc -c -o $@ $(GCC_OPTS) Dconfig.c	

Ddictionary.o: Ddictionary.h Ddictionary.c
	gcc -c -o $@ $(GCC_OPTS) Ddictionary.c

Dnode.o: Dnode.h Dnode.c
	gcc -c -o $@ $(GCC_OPTS) Dnode.c

DnodeList.o: DnodeList.h DnodeList.c
	gcc -c -o $@ $(GCC_OPTS) DnodeList.c


# Build targets

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g"

doc: Makedoc
	make -f Makedoc

clean:
	rm -rf *.o
	rm dictionnaire