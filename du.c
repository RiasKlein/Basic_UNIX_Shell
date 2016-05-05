/******************************************************************************

	Name: 		Shunman Tse
	NID:		N13382418
	Net ID:		st1637
	Course:		UNIX Programming
	Homework:	5: Implement the utility du

	du.c

******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define NULL_INPUT 0		// Used when user doesn't provide path input
#define PATH_INPUT 1		// Used when user does provide path input
#define MAX_PATH 2000000	// Larger than it needs to be... I believe...

#define WALK_F  1			// Flag for file that isn't a directory
#define WALK_D  2			// Flag for directories
#define WALK_NR 3			// Flag for directory that cannot be read
#define WALK_NS 4			// Flag for file that we cannot use lstat on

/* Function Prototypes */
int count_block (char* pathname, struct stat *statptr, int flag, long* blocks);
void du_print (long blocks, char* path);
int walk (char* path, long* blocks, long* ino, long* n_ino);
int walk_init (char* starting_path, int path_provided);

int main (int argc, char** argv) {
	
	// If no argument is provided, pass the flag NULL_INPUT to walk_init
	// Otherwise, pass the flag PATH_INPUT to walk_init
	if (!argv[1]) {
		walk_init (argv[1], NULL_INPUT);
	}
	else {
		walk_init (argv[1], PATH_INPUT);
	}

	return 0;
}

/*
	Handles increasing the counter for blocks allocated
	Also prints out error messages when necessary (depends on flag)
*/
int count_block (char* pathname, struct stat *statptr, int flag, long* blocks){
	switch (flag) {
		case WALK_F:
			*blocks += statptr->st_blocks;
			break;
		case WALK_D:
			*blocks += statptr->st_blocks;
			break;
		case WALK_NR:
			printf ("Cannot read directory ");
			du_print ((long)-1, pathname);
			du_print (((long)statptr->st_blocks), pathname);
			break;
		case WALK_NS:
			printf ("Stat error for %s\n", pathname);
			break;
		default:
			printf ("Unknown flag %d for pathname %s\n", flag, pathname);
	}

	return (0);
}

/*
	Prints output
	In the case that blocks count is positive, output that normally
	Negative block counts is for error message prints
*/
void du_print (long blocks, char* path) {
	if (blocks < 0) {
		if (path[0] == '/' && path[1] == '/') {
			printf ("'%s'\n", path+1);
		}
		else {
			printf ("'%s'\n", path);
		}
	}
	else {
		if (path[0] == '/' && path[1] == '/') {
			printf ("%ld\t%s\n", (blocks/2), path+1);
		}
		else {
			printf ("%ld\t%s\n", (blocks/2), path);
		}
	}
}

/*
	Descend through the file hierarchy with path as the starting point
	Steps:
	- 	Call lstat to populate stat structure
	- 	Treat the file differently depending on its type; that is whether it's
			a file or a directory
	- 	Files with a single link are counted normally
		Files with more than one link are checked against records in ino
			before they are counted in order to prevent overcounting
	- 	Directories are opened and counted (recursively)
*/
int walk (char* path, long* blocks, long* ino, long* n_ino) {
	struct stat sb;
	struct dirent *dirp;
	DIR *dp;
	int success;
	char* ptr;

	// Call lstat on provided path to populate stat struct
	if (lstat(path, &sb) == -1) // stat error
		return (count_block (path, &sb, WALK_NS, blocks));

	// If our file is NOT a directory, we want to check the value of st_nlink
	// Files that only have a single hardlink are counted normally
	// Files with more than one hardlink are checked with ino to prevent 
	// 	overcounting issue; first encounter: add into ino & count it
	if (S_ISDIR(sb.st_mode) == 0){ 
		if (sb.st_nlink > 1) {
			int found = 0;
			for (long i = 0; i < *n_ino; i++) { // Check with ino
				if (ino[i] == (long)(sb.st_ino)) {
					found = 1;
					return 0;
				}
			}
			if (found == 0) { // First encounter with this file inode
				*n_ino += 1;
				if (*n_ino > MAX_PATH) {
					ino = (long*) realloc (ino, *n_ino * sizeof(long));
				}
				ino[*n_ino-1] = (long)(sb.st_ino);
				return (count_block (path, &sb, WALK_F, blocks));
			}
		}
		else { // 1 st_nlink is just counted normally
			return (count_block (path, &sb, WALK_F, blocks));
		}
	}

	// Getting here means that we have a directory
	long before_blocks = *blocks; // Number of blocks before recursion

	if ((success = count_block (path, &sb, WALK_D, blocks)) != 0)
		return success;

	ptr = path + strlen(path); 	// Point to the end of path
	*ptr++ = '/';				// Add a / to the end of path
	*ptr = 0;
	
	if ((dp = opendir (path)) == NULL) // Can't read directory
		return (count_block (path, &sb, WALK_NR, blocks));

	// Count files & go into directories recursively
	// Ignore files with name "." or '..'
	while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
			continue;
		strcpy(ptr, dirp->d_name); // add file name after the slash
		if (walk (path, blocks, ino, n_ino) != 0)
			break;
	}
	
	ptr[-1] ='\0'; 				// Cleanup: back up on the "/"
	if (closedir(dp) < 0) { 	// Close the directory
		printf ("Cannot close directory: %s", path);
	}

	// Print out the result of the count procedure (after recursion)
	long internal_blocks = *blocks - before_blocks;
	du_print (internal_blocks, path);

	return success;
}

/*
	Set-up a filepath buffer (path) for file hierarchy traversal
	-	path contains starting directory for the walk 
			memory is allocated for path because it will change over the 
			course of the file hierarchy traversal
	- 	ino starts off with a rather large initial size and it can increase
			as needed (via realloc)
*/
int walk_init (char* starting_path, int path_provided) {
	long blocks;
	long n_ino = 0;
	long* ino = (long*) malloc (MAX_PATH * sizeof (long));

	// Create a buffer to hold the full path of the files we look at
	char* path = (char*) malloc (MAX_PATH * sizeof (char));

	if (path_provided == NULL_INPUT) {
		// The path should be the current directory if there wasn't an input
		path[0] = '.';
		path[1] = '\0';
	}
	if (path_provided) {
		// Copy starting_path to path
		strncpy (path, starting_path, MAX_PATH);
		path[MAX_PATH-1]='\0'; // Protect against buffer overrun
	}

	// Call walk to get the show on the road! (It does... basically everything)
	walk (path, &blocks, ino, &n_ino);

	// Cleanup: free path & ino
	free (path);
	free (ino);

	return 0;
}