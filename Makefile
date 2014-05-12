GCC_OPTS = -O3

.PHONY: doc

all: main.c application.o Dconfig.o Ddictionary.o Dnode.o Dword.o DwordList.o
	gcc -o dictionnaire $(GCC_OPTS) main.c application.o Dconfig.o DwordList.o \
	Dword.o Dnode.o Ddictionary.o

application.o: application.c
	gcc -c -o $@ $(GCC_OPTS) application.c	

Dconfig.o: Dconfig.h Dconfig.c
	gcc -c -o $@ $(GCC_OPTS) Dconfig.c

Ddictionary.o: Ddictionary.h Ddictionary.c
	gcc -c -o $@ $(GCC_OPTS) Ddictionary.c

Dnode.o: Dnode.h Dnode.c
	gcc -c -o $@ $(GCC_OPTS) Dnode.c

Dword.o: Dword.h Dword.c
	gcc -c -o $@ $(GCC_OPTS) Dword.c

DwordList.o: DwordList.h DwordList.c
	gcc -c -o $@ $(GCC_OPTS) DwordList.c

# Build targets

dev:
	make GCC_OPTS="-Wall -Wextra -DDEBUG -g"

doc: Makedoc
	make -f Makedoc

clean:
	rm -rf *.o
	rm dictionnaire