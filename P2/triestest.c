#include "tries.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define WORDLENGTH 17
/**
 * This is a testclient to test the functionality of the tries
 * implementation. tests for sanitition faults, segmentation,
 * and program runs as intended.
 *
 * @author Arun Felix, Kareem Jackson
*/
char** randomwords;
char* randomword(){
    char* word = (char*)malloc(sizeof(char) * WORDLENGTH);
    for(int i = 0; i < WORDLENGTH - 1; i++){

        word[i] = (rand() % 26) + 97;
        // printf("our random character%c\n",word[i]);
    }
    word[WORDLENGTH - 1] = '\0';
    printf("our random word is: %s\n",word);
    return word;
}
int main(int argc, char** argv){
    char* myptr = "hatsune miku";
    put(myptr);
    char* ptr2 = "HatsuNe miku";
    printf("%s\n",get(ptr2));
    destroy();
}