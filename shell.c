/******************************************************************************

	Name: 		Shunman Tse
	
	Simple Shell

******************************************************************************/

#include <stdio.h>		// printf, perror
#include <stdlib.h>		// exit
#include <string.h>		// strtok, strcmp
#include <unistd.h>		// fork
#include <sys/wait.h>	// wait

#define ARG_MAX 2000000

/* Function Prototypes */
void built_in (char** argv);
void exec_argv (char** new_argv);
char** gen_argv (char* command);
void print_prompt(char* username);
char* prompt_init(void);

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
		char** new_argv = gen_argv (command);

		// Check if the command is a built-in function
		built_in(new_argv); 

		// Exec the generated argv
		exec_argv (new_argv);
	}

	return 0;
}

/*
	Check if the command is one of our built-in functions
	The only built in function this time is exit
*/
void built_in (char** argv) {
	int cmpresult = strcmp (argv[0], "exit");	// Is the command exit?
	if (cmpresult == 0) { 	// Command == exit
		exit(0);			// Exit 
	}
}

/*
	Call exec with the provided argv (char** new_argv)
*/
void exec_argv (char** new_argv) {
	// Make sure we don't attempt to exec an empty command
	if (strcmp(new_argv[0], "") != 0) { 
		// The child execs the user's command
		if (fork() == 0) {
			int exec_success = execvp (new_argv[0], new_argv);
			if (exec_success == -1) {
				perror ("Exec call failed");
				exit(2);
			}
		}
		// The parent waits for the child
		else {
			wait(NULL);
		}
	}
}

/*
	Convert command (string) into argv array
	Realloc is used to dynamically increase the size of argv as needed
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
	argv[0] = token;
	
	// Add tokens into char** argv array (realloc to make space for tokens)
	token = strtok (NULL, delim);
	while (token != NULL) {
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
	argv[size-2][strlen(argv[size-2])-1] = '\0'; // End final token with '\0'
	argv[size-1] = NULL;				// End argv with NULL entry

	return argv;
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
*/
char* prompt_init(void) {
	char* username = getenv ("PS1");	// Use PS1's value by default
	if (!username) {					
		username = getenv ("USER");		// Use USER's value otherwise
	}
	return username;
}
