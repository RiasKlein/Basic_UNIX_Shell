/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	4: Piping

	checks.c
		Contains functions for error checking

******************************************************************************/

#include <stdio.h>		// printf, perror
#include <ctype.h>		// isspace
#include <stdlib.h>		// exit
#include <string.h>		// strtok, strcmp
#include <unistd.h>		// fork, chdir
#include <sys/types.h>	// pid_t
#include <sys/wait.h>	// wait
#include <glob.h>		// glob
#include "checks.h"		// header file

/*
	Checks the result of exec call
	Prints error and exits if failure
*/
void check_exec(int exec_success) {
	if (exec_success == -1) {
		perror ("Exec failed");
		exit(2);
	}
}

void check_flush(int flush_success) {
	if (flush_success == EOF) {
		perror ("Stdout Flush error");
		exit (2);
	}
}

/*
	Checks the result of fork call
	Prints error and exits if failure
*/
void check_fork(pid_t pid) {
	if (pid == -1) {
		perror ("Fork failed");
		exit(2);
	}
}

/*
	Checks the result of pipe call
	Prints error and exits if failure
*/
void check_pipe(int pipe_success) {
	if (pipe_success == -1) {
		perror ("Pipe failed");
		exit(2);
	}
}

void check_sa (int sa_success) {
	if (sa_success == -1) {
		perror ("Sigaction error");
		exit(2);
	}
}

/*
	Checks the result of wait call
	Prints error and exits if failure
*/
void check_wait (int wait_success) {
	if (wait_success == -1) {
		perror("Wait error");
		exit(2);
	}
}