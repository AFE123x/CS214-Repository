#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)
/**
 * another memtesting client for filling heap with data, than freeing data.
 * Ensures data is accessible, and works as intended, changing data ONLY in
 * payload.
 * @author Arun Felix & Kareem Jackson
 * @return condition code.
*/
int main(int argc, char **argv)
{
	char *obj[OBJECTS];
	int i, j, errors = 0;
	
	// fill memory with objects
	for (i = 0; i < OBJECTS; i++) {
		obj[i] = malloc(OBJSIZE);
	}
	clienttrouble();
	// fill each object with distinct bytes
	for (i = 0; i < OBJECTS; i++) {
		memset(obj[i], i, OBJSIZE);
	}
	
	// check that all objects contain the correct bytes
	for (i = 0; i < OBJECTS; i++) {
		for (j = 0; j < OBJSIZE; j++) {
			if (obj[i][j] != i) {
				errors++;
				printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
			}
		}
	}
	
	printf("%d incorrect bytes\n", errors);
	for (i = 0; i < OBJECTS; i++) {
		free(obj[i]);
	}
	clienttrouble();
	
	return EXIT_SUCCESS;
}
