#ifndef TRIES_H
#define TRIES_H
#include<string.h>

void put(char* word); //putting string in tries
char* get(char* word); //returns word if success, NULL if fail?
void destroy(); //clean our tries, free memory.
char exists(char* word); //return 1 if true, 0 if false.
#endif