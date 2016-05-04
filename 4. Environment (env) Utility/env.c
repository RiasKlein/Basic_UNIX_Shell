/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	2: The Environment 

	Version:	1.00

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

extern char** environ;

/* Function prototypes */
void print_env ();
int count_env ();
char** gen_env (int env_size, int new_env_flag);
int name_match (char* s1, char* s2);
int exec_arg_count (char** argv);

/*
	print_env
	Prints out the elements of the environ array
*/
void print_env () {
	for (int i = 0; environ[i] != NULL; i++) {
		puts(environ[i]);
	}
}

/*
	count_env
	Returns the number of the elements in the environ array
*/
int count_env () {
	int size = 0;

	// Count all elements up until NULL
	for (int i = 0; environ[i] != NULL; i++) {
		size++;
	}

	return size;
}

/*
	gen_env
	Dynamically allocate memory for a new environment array
	Provided flag determines whether to copy old env array or not
*/
char** gen_env (int env_size, int new_env_flag) {
	char** return_ptr = (char**) malloc ( (env_size+1) * sizeof (char*));

	// Note: We have a +1 to the size of env_size because we are making 
	// space for the NULL pointer at the end of the environ array

	// Check that we were able to allocate the new env array
	if (!return_ptr){
		fprintf (stderr, "Failed to allocate new environment array\n");
		exit(1);
	}

	// The environment array is NULL terminated
	for (int i = 0; i < env_size+1; i++) {
		return_ptr[i] = NULL;
	}

	// Carry over the old array if necessary
	if (!new_env_flag) {
		for (int i = 0; environ[i] != NULL; i++) {
			return_ptr[i] = environ[i];
		}
	}

	return return_ptr;
}

/*
	name_match
	Compares two (name=value) char arrays for matching names
*/
int name_match (char* s1, char* s2) {
	int match = 1;

	// For-loop to match the two provided "strings" one char at a time
	// The matching process goes until we reach the = to mark the end of name
	for (int i = 0; (s1[i] != '=') && (s2[i] != '='); i++) {
		if (s1[i] == s2[i]) {
			match = 1;
		}
		else {
			match = 0;
			break;
		}

		if ((s1[i+1] == '=' && s2[i+1] != '=') || (s1[i+1] != '=' && s2[i+1] == '=')) {
			match = 0;
			break;
		}
	}

	return match;
}

/*
	exec_arg_count
	Counts number of entries needed for arguments array to pass to exec call
*/
int exec_arg_count (char** argv) {
	int argument_count = 0;

	// The exec arguments start with the command and follows with arguments
	// They show up after the name=value pairs to add to the environ array
	for (int i = 1; argv[i] != NULL; i++) {
		char* pair = strchr (argv[i], '=');
		if (!pair) {

			for (int j = i; argv[j] != NULL; j++) {
				argument_count++;
			}
			break;
		}
	}
	argument_count++; // Account for the NULL termination at end of arguments
	
	return argument_count;
}

int main(int argc, char **argv) {
	int index = 0;			// index to start reading arguments from
	int new_env_flag = 0;	// Determines if we need to create new env array
	int env_size = 0;		// Used to determine new env array size 
	int env_index = 0;		// environment array index

	// Determine whether we need to modify the environment or not
	// If no command is provided then env will simply display the environment
	if (argc > 1) {
		// Getting here means that we need to modify the environment 
		// To do that, we will create a new array for environ

		// Check if -i is an argument in position argv[1]
		// If -i is used, that means we must completely replace the environment
		if (strcmp (argv[1], "-i") == 0) {
			new_env_flag = 1;
			index = 2;
		}
		else {
			index = 1;
		}	

		// Count the number of additional elements we are adding to the env
		for (int i = index; argv[i] != NULL; i++) {
			char* pair = strchr (argv[i], '=');
			if (!pair) {
				// NULL result, meaning there wasn't a name=value pair
				break;
			}
			else {
				env_size++;
			}
		}

		// If we are not making a completely new env
		// then we need to consider the size of the existing env
		if (!new_env_flag) {
			env_index = count_env();
			env_size += env_index;
		}

		// Generate the new envionment array
		char** newenv = gen_env(env_size, new_env_flag);

		// Add new (name, value) pairs to the env array
		for (int i = index; argv[i] != NULL; i++) {
			char* pair = strchr (argv[i], '=');
			if (!pair) {
				break;
			}
			else {
				int replaced = 0;

				int len = strlen (argv[i]);
				char* replace = (char*) malloc ( (len+1) * sizeof (char));
				if (!replace) {
					fprintf (stderr, "Failed to allocate replace array\n");
					exit(1);
				}
				replace[len] = '\0';
				for (int k = 0; k < len; k++) {
					replace[k] = argv[i][k];
				}

				// Check whether name is already in the env array
				for (int j = 0; newenv[j] != NULL; j++) {
					if (name_match(newenv[j], argv[i])) {
						// We got a match, replace with proper value
						newenv[j] = replace;
						replaced = 1;
					}
				}

				// Name was NOT in the env array
				// We must add the element 
				if (!replaced) {
					newenv[env_index] = replace;
					env_index++;
				}
			}
		}

		// Set the newly generated env array to be the environ array
		environ = newenv;
	}

	// Now, we want to exec the provided command (if any)
	for (int i = index; argv[i] != NULL; i++) {
		char* pair = strchr (argv[i], '=');
		if (!pair) {
			// argv[i] is the command we are looking for
			
			// Generate the proper arguments array
			int num_args = exec_arg_count(argv);
			char** exec_arg = (char**) malloc (num_args * sizeof(char*));
			for (int j = 0; argv[i+j] != NULL; j++) {
				exec_arg[j] = argv[i+j];
			}
			exec_arg[num_args-1] = NULL;

			// Call exec
			int exec_success = execvp (argv[i], exec_arg);
			if (exec_success == -1) {
				printf ("./env: %s: No such file or directory\n", argv[i]);
				exit(1);
			}
		}
	}

	// We didn't find a command so we should just print out environ array
	print_env();

	return 0;
}