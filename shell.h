/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	4: Piping

	shell.h
		Contains prototypes for functions in shell.c

******************************************************************************/

/* Prototypes for Shell functions */

int built_in (char** argv);
void cd_command (char** argv);
int count_pipe(char* command);
void exec_argv (char** new_argv);
char** gen_argv (char* command);
char** gen_argv_io (char** argv, int new_size);
char*** gen_argv_pipe (char* command, int num_pipes);
char** handle_io_redir2 (char** argv);
void no_pipe_exec (char* command);
void pipe_exec (char* command, int num_pipes);
void pipe_exec_argv (char** new_argv);
void print_argv2 (char** argv);
void print_gen_argv_io (char** argv, char** new_argv);
void print_prompt(char* username);
void print_child_term (int status);
char* prompt_init(void);
void signal_handler (int signum, siginfo_t *siginfo, void *context);
void signal_init ();
void signal_prompt ();
void signal_reset();
void wait_for_children (int num_pipes);