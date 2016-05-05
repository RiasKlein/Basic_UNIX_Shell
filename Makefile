FLAGS = -std=c99 -Wall -O1 -D _POSIX_C_SOURCE=200112L

all: du

du: du.c 
	gcc ${FLAGS} -o du du.c 

clean:
	rm -f du
	rm -f core