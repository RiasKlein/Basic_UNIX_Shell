FLAGS = -std=c99 -Wall -O1

all: shell

shell: shell.c
	gcc ${FLAGS} -o shell shell.c

clean:
	rm -f shell
	rm -f core