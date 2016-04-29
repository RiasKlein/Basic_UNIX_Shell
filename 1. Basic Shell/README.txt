/******************************************************************************

	Name: 		Shunman Tse
	
	Basic Process Control (simple shell)

	Version:	1.00

******************************************************************************/

Focus: Process Control

The program is a simple shell.
It accepts commands, including arguments and executes them.

Built-in Commands:
1.	exit: The shell will exit when the command exit is used.

Not included:
1. 	Piping
2. 	I/O Redirection
3. 	Background Processing
4. 	Globbing
5.	Quotes

Notes:
1.	The user's input size should be less than ARG_MAX which equals 2000000
2. 	The status argument for wait() is not utilized

	