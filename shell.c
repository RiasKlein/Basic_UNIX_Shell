/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	4: Piping

	shell.c

******************************************************************************/

#include <stdio.h>		// printf, perror
#include <ctype.h>		// isspace
#include <stdlib.h>		// exit
#include <string.h>		// strtok, strcmp
#include <unistd.h>		// fork, chdir
#include <sys/types.h>	// pid_t
#include <sys/wait.h>	// wait
#include "checks.h"		// header file for check functions
#include "shell.h"		// header file for shell functions

#define ARG_MAX 2000000

int main(int argc, char **argv) {
	char* command = (char*) malloc (ARG_MAX * sizeof(char)); // Command Buffer
	if (!command) {
		perror ("Malloc call for command failed. Terminating Program.");
		exit(1);
	}
	char* username = prompt_init(); 	// Initialize prompt statement

	// Prompt and execute commands until user wants to exit
	while (1) {
		print_prompt(username);
		command = fgets(command, ARG_MAX, stdin);

		// Count the number of pipes to determine if we need piping
		int num_pipes = count_pipe(command);

		if (num_pipes > 0) { 	// There is at least one pipe
			pipe_exec (command, num_pipes);
		}
		else { 					// No pipes in command
			no_pipe_exec (command);
		}
	}

	// Free Allocated Memory
	free (command);

	return 0;
}

/*
	Check if the command is one of our built-in functions
	Return 1 if the command is a built-in command
	Return 0 otherwise
*/
int built_in (char** argv) {
	int cmpresult = strcmp (argv[0], "exit");	// Is the command exit?
	if (cmpresult == 0) { 						// Command == exit
		exit(0);								// Exit 
	}

	cmpresult = strcmp (argv[0], "cd");			// Is the command cd?
	if (cmpresult == 0) {						// Command == cd
		cd_command (argv);						// Handle the cd command
		return 1;
	}

	return 0;
}

/*
	Handles cd command
	Changes directory to what is specified in argv[1] (path)
	If no path is specified, go to the HOME variable in env
	If HOME is not defined, 
*/
void cd_command (char** argv) {
	int cdresult = -2;

	// Consider the case that there isn't a path
	char* path = argv[1];
	if (argv[1] == NULL) {
		path = getenv("HOME");
		if (path == NULL) {
			printf ("ERROR: Environment variable HOME is not set\n");
			printf ("To use cd, provide a path: cd [path]\n");
		}
		else {
			cdresult = chdir (path);
		}
	}
	else {
		cdresult = chdir (path);
	}
	
	if (cdresult == -1) {
		perror ("cd command failed");
	}
}

/*
	Checks the command to see if we have piping
	Returns the number of pipes
*/
int count_pipe(char* command) {
	int pipes = 0;

	// Don't attempt to count for pipes if the command is NULL
	if (command != NULL) {
		int command_length = strlen(command);

		// If we find a | character, increment pipes counter
		for (int i = 0; i < command_length; i++) {
			if (command[i] == '|') {
				pipes++;
			}
		}
	}	

	return pipes;
}

/*
	Call exec with the provided argv (char** new_argv)
*/
void exec_argv (char** new_argv) {
	// Make sure we don't attempt to exec an empty command
	if (new_argv != NULL && (strcmp(new_argv[0], "") != 0) ) { 
		// The child execs the user's command
		if (fork() == 0) {
			int exec_success = execvp (new_argv[0], new_argv);
			check_exec(exec_success);
		}
		// The parent waits for the child
		else {
			int wait_success = wait(NULL);
			check_wait (wait_success);
		}
	}
}

/*
	Convert command (char*) into argv array
	Realloc is used to dynamically increase the size of argv as needed
	Used when command contains only one argv to be extracted
*/
char** gen_argv (char* command) {
	char* delim = " "; 		// Initialize delimiter for commands
	int size = 2;			// Size of the argv array

	char** argv = (char**) malloc (size * sizeof(char*)); 
	if (!argv) {
		perror ("Malloc call for argv[] failed. Terminating program.");
		exit(1);
	}

	char* token = strtok (command, delim);	// Use strtok for token
	if (token == NULL) return NULL;
	argv[0] = token;

	// Add tokens into char** argv array (realloc to make space for tokens)
	token = strtok (NULL, delim);
	while ( (token != NULL) && ((strcmp(token, "\n") != 0) )) {
		// Resize char** argv so it can hold another token entry
		size++;
		argv = (char**) realloc (argv, size * sizeof (char*));
		if (!argv) {
			perror ("Realloc call for argv[] failed. Terminating program.");
			exit(1);
		}

		argv[size-2] = token;			// Add current token to argv[]
		token = strtok (NULL, delim);	// Get the next token
	}

	// When users input a command, there is a \n in it, turn it to \0
	if (argv[size-2][strlen(argv[size-2])-1] == '\n')
		argv[size-2][strlen(argv[size-2])-1] = '\0'; // End with '\0'
	
	argv[size-1] = NULL;				// End argv with NULL entry

	return argv;
}

/*
	Convert command (char*) into argv arrays
	Used when command contains pipes and therefore multiple argv's
	This function strtoks using the delimiter "|" and then " "
*/
char*** gen_argv_pipe (char* command, int num_pipes) {
	char* n_delim = "|";	// Initialize delimiter | (Pipe)
	int i = 1;

	// commands contain the commands of char* command 
	char** commands = (char**) malloc ( (num_pipes + 1) * sizeof(char*));
	if (!commands) {
		perror ("Malloc call for commands[] failed. Terminating program.");
		exit(1);
	}

	// Find the individual commands using "|" as a delimiter
	char* token = strtok (command, n_delim); 
	commands[0] = token;
	token = strtok (NULL, n_delim);
	while ((token != NULL) && ((strcmp(token, "\n") != 0))) {
		while(isspace(*token)) token++;
		commands[i] = token;
		i++;
		token = strtok (NULL, n_delim);
	}
	
	// new_argv contains the argvs of the entities in char** commands
	char*** new_argv = (char***) malloc ( (num_pipes + 1) * sizeof(char**));
	if (!new_argv) {
		perror ("Malloc call for new_argv[] failed. Terminating program.");
		exit(1);
	}

	// Use gen_argv() to turn white-space containing commands in commands
	// to individual elements
	for (int i = 0; i < num_pipes + 1; i++) {
		new_argv[i] = gen_argv(commands[i]);
	}	

	// Free commands
	free (commands);

	return new_argv;
}

/*
	Executes provided command: (no piping)
	- converts command into argv array
	- checks if command is a built-in function
	- execs the command
*/
void no_pipe_exec (char* command) {
	char** new_argv = gen_argv (command);

	// Make sure that we don't try to exec a NULL argv
	// Check if the command is a built-in function
	// Exec the generated argv if it isn't a built-in function
	if (new_argv != NULL) { 
		if (!built_in(new_argv)) {
			exec_argv (new_argv);
		}
	}
	else {
		perror ("Error: Empty command cannot exec");
		exit(1);
	}

	// Free new_argv
	free(new_argv);
}

/*
	Executes provided command: (with piping)
	- converts command into argv arrays
	- generates pipes and executes commands
	- children produced via fork() are collected at the end
*/
void pipe_exec (char* command, int num_pipes) {
	// new_argv[i] is an argv array for exec
	char*** new_argv = gen_argv_pipe(command, num_pipes);

	// pipefd[0] = read end
	// pipefd[1] = write end
	int pipefd[2];
	int next_stdin = -1;		// Contains FD of input for next child
	pid_t pid;

	// Children are produced in a fanning manner
	// That is the parent produces the children and the children execs
	for (int i = 0; i < num_pipes + 1; i++) {
		int pipe_success = pipe(pipefd); 	// Create pipe
		check_pipe(pipe_success);			// Check pipe

		int my_stdin = pipefd[0]; 	// Get read end of my pipe
		// The first child wouldn't have anything coming in
		if (i > 0) pipefd[0] = next_stdin; 
		next_stdin = my_stdin;		// Next child's read end is my_stdin

		pid = fork();				// Fork
		check_fork(pid);			// Check the output of fork()
		if (pid == 0) {				// Child 

			// If this is not the first child in the pipeline
			// Dupe pipe read end to FD for input (0)
			if (i > 0) {
				dup2(pipefd[0], 0);
			}
			close(pipefd[0]); // Close the pipe read end

			// If this isn't the end of the pipeline
			// Dupe pipe's write end to FD for output (1)
			if (i < num_pipes) {
				dup2(pipefd[1], 1);
			}
			close (pipefd[1]);	// Close the pipe write end

			// Now that the pipes are set up, exec the command
			pipe_exec_argv(new_argv[i]);
		}
		else { // Parent
			// If this is not the first in the pipeline
			// Close the read end of the pipe
			if (i > 0) {
				close (pipefd[0]);
			}

			// Close the write end of the pipe since we won't use
			close(pipefd[1]);

			// If this is the end of the pipeline, there is no 
			// next child to take a read end, so we just close it
			if (i == num_pipes) {
				close(next_stdin);
			}		
		}
	}

	// Wait for all the children forked above to exit
	wait_for_children (num_pipes);
	
	// Free new_argv
	free (new_argv);
}

/*
	Execs a provided new_argv
	- checks to see if we have a built-in command
		- if so, handle it and exit
	- exec the command
*/
void pipe_exec_argv (char** new_argv) {
	//if (new_argv != NULL) {
	if (new_argv != NULL && (strcmp(new_argv[0], "") != 0) ) { 
		if (!built_in(new_argv)) {
			int exec_success = execvp (new_argv[0], new_argv);
			check_exec(exec_success);
		}
		else {
			exit(0);
		}
	}
	else {
		printf ("Error: Empty command cannot exec\n");
		exit(1);
	}
}

/*
	Prints prompt statement
*/
void print_prompt(char* username) {
	printf ("%s: ", username);
}

/*
	Determine the prompt statement
	If PS1 is present, use that as the prompt statement
	Else, use the username (USER) found in the env
	If all else fails, use "shell" as the username
*/
char* prompt_init(void) {
	char* username = getenv ("PS1");	// Use PS1's value by default
	if (!username) {					
		username = getenv ("USER");		// Use USER's value otherwise
	}	
	if (!username) {
		username = "shell";				// Use "shell" as final option
	}
	
	return username;
}

/*
	Wait for children to exit
	The number of children generated is num_pipes + 1
	i.e. child1 | child2 | child3
*/
void wait_for_children (int num_pipes) {
	for (int i = 0; i < num_pipes + 1; i++) {
		int wait_success = wait(NULL);
		check_wait (wait_success);	
	}
}