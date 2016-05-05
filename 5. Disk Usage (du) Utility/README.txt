/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	5: Implement the utility du

	Version:	1.1

******************************************************************************/

Focus:
1.	File System Hierachy
2. 	File "status"

This program implements the utility du.
The intended platform is Linux.

Regarding changes since version 1.0:
1.	Added -D _POSIX_C_SOURCE=200112L to Makefile for lstat
2.	Added printing function to handle printing output

Regarding the implementation:

1.	The starting path is determined by the user's input (or lack of input). 
2 	As we step into directories and sum up block sizes, non-directory files are
		checked for hard links and files with hard links have their inode value
		stored in an array called ino. If a file is encountered and its inode
		is already stored in ino, the blocks allocated to it are ignored; this
		is done to prevent overcounting.
3. 	lstat is used instead of stat so we get inode information of symbolic links
		instead of getting the inode information of the file the symbolic link
		refers to. 

Notes:
1. 	Testing was done by comparing the real du results with the created du
2.	Test cases used to check the du program were:
		./du /
		./du /home/<username>
		./du

		