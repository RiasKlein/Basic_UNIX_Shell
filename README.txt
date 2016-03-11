/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	4: Piping

	Version:	1.0 

******************************************************************************/

Focus: 
1.	Piping
2. 	cd command

Regarding cd command:
1. 	If cd is run without a provided path, it goes to HOME variable in env
2. 	If HOME is not defined, then a usage message is printed saying that the 
	env variable for HOME has not been defined and that to use cd, 
	input: cd [path]

Regarding piping:
1.	Long chains of pipes are supported
2.	Testing was done by comparing regular shell results with created shell
3. 	Test cases used to check piping were up to 3 entities chained, e.g.
	ls -X | sort -X | less / more
	man | X | less / more

Notes:
1.	Function gen_argv() checks the very last argument of a command for the
	character "\n" and changes it to "\0" since the delimiter used for
	tokening is " " and it does not change "\n".
	The "\n" is seen at the end of every command input.
2.	The prompt "username" has been updated so that it prints shell as
	the username in case both PS1 and USER env variables are not defined.
3.	Output checking functions have been moved to source file: checks.c
4.	Function prototypes have been moved to appropriate header files:
	checks.h and shell.h

