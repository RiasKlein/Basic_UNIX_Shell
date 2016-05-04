/******************************************************************************

	Name: 		Shunman Tse
	
	Environment (Env) Utility

	Version:	1.00

******************************************************************************/

Focus: 
1. Process environment
2. Memory management

This program implements the UNIX utility env.

If the program is executed without any arguments, then it will simply display
the environment.

With arguments, the usage is:
	env [-i] [name=value]... [command [argument]...]

The -i option is used to completely replace the environment with the provided
name=value pairs in the command and then execute the given command in the 
modified environment.

If -i is not provided, then the provided name=value pairs will be added to
the existing environment and then the command would be executed in the 
modified environment.

If no command is given, then env will simply display the modified environment.

	