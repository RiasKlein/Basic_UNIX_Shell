/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	4: Piping

	checks.h
		Contains prototypes for functions in checks.c

******************************************************************************/

/* Prototypes for Check functions */

void check_exec(int exec_success);
void check_fork(pid_t pid);
void check_pipe (int pipe_success);
void check_wait (int wait_success);