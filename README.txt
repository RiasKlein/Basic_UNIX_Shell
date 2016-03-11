/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	6. I/O Redirection, Signals, Globbing

	Version:	1.0 

******************************************************************************/

Focus:
1.	I/O Redirection
2.	Signals
3. 	Globbing

The features mentioned above are added to a basic shell.

Regarding I/O Redirection:
1. 	Support basic use of >, >>, & <
2. 	Functions with piping
3.	Some test cases used:
	ls -l *.c > file
	ls -l *.c ../*/*.c *.? >> file
	ls -l ../*/*.c | sort > file
	ls -l ../*/*.c | sort -r >> file
4. 	Regarding the implementation, instead of opening the desired filename and 
	duping to the proper file descriptor and then closing the file descriptor
	where the file originally opened, I simply closed the desired destination
	and then opened the file. 
	Ex. If I wanted to open a file in stdout, I would use close(1); and then
	I would call open, which should open the file in fd 1 since it's the lowest
	available fd.
5. 	File descriptor redirection was not implemented.
	That is, ls 1> file and commands of that form, are not supported.
6. 	Assumption: There will be a space before and after IO redirection operators
	Ex. ls > file is supported		however, ls>file is NOT supported

Regarding Signals:
1.	There was difficulty getting my shell's prompt output to match that of the
	real shell's when dealing with signals. For example, if a SIGINT were 
	received via Ctrl+C, there would be a ^C on the screen, but the prompt 
	doesn't display again and we wouldn't be on a new line. Therefore, it was
	possible to have: 
		prompt: ^C^C^C^C^C
	The prompt wouldn't print again until ENTER was pressed.
	In order to tackle this, I added a print statement in a signal handler.
	I understand that's not a good idea, but I was running out of ideas on how
	to make the two shells have the same output.
2.	The backend implementation is functional. The shell handles SIGINT & SIGQUIT
	but the child used to exec commands does not handle those two signals. 

Regarding Globbing:
1.	Some test cases used:
	ls -l c*.?
	ls -l *.c ../*/*.c
	ls -l [a-d]*.c
	ls -l *.c | tee teeo* | wc 
	ls -l ../*/*.c | sort | tee teeo* | wc
2. 	There are issues with commands that include their own globbing such as find.


