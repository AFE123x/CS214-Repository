#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/**
 * A better isalpha implementation because the ctype one sucks. 
 * @arg input: a character we want to check, if it's an alpha character
 * @return 1 if character is alpha, 0 if not. 
*/
char betterisalpha(char input){
    return (input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z');
}

/**
 * This function is intended to take a word with non alpha characters on the side, and trim it
 * @arg word - a word containing the trailing characters in the begginning and end.
 * @return trimmed down string, NULL if string contains no alpha characters
*/
char* chopword(char* word) {
    if (!strlen(word)) {
        return NULL;
    }
    //remove trialing stuff
    unsigned int R = strlen(word) - 1;
    unsigned int L = 0;
    while (!betterisalpha(word[L]) && L <= R) L++;
    while (!betterisalpha(word[R]) && L <= R) R--;
    if(L > R){
        free(word);
        return NULL;

    }
    printf("L = %d\tR = %d\n", L, R);
    memmove(word, word + L, ((R - L) + 1));
    word[(R - L) + 1] = '\0';
    printf("newstring = %s\n", word);
    //pe-nis\0
    //  i
    unsigned int length = strlen(word);
    for(int i = 0; word[i] != '\0'; i++){
        if(word[i] == '-'){
            memmove((word + i),(word + i + 1),(length - i));
        }
    }
    char* oldptr = word;
    word = realloc(word,sizeof(char) * (strlen(word) + 1));
    if(word == NULL){
        free(oldptr);
        exit(EXIT_FAILURE);
    }
    printf("%s has length %lu\n",word,strlen(word));
    return word;
}

int main(int argc, char** argv){
    if(argc < 2){
        fprintf(stderr,"./trimword {word}\n");
        return EXIT_FAILURE;
    }
    int length = strlen(argv[1]);
    char* mystring = malloc(sizeof(char) * (length + 1));
    memcpy(mystring,argv[1],length + 1);
    printf("length of string: %d\n",length);
    mystring = chopword(mystring);
    if(mystring == NULL){
        return EXIT_FAILURE;
    }
    free(mystring);
    
}