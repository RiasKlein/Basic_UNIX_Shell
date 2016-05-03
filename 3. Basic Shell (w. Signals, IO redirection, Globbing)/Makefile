FLAGS = -std=c99 -Wall -O1 -D_POSIX_C_SOURCE=200809L

all: shell

shell: shell.c shell.h checks.c checks.h
	gcc ${FLAGS} -o shell shell.c checks.c

clean:
	rm -f shell
	rm -f core