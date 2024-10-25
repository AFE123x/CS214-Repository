/*
Contains the functions used to parse a file or parse a dictionary

|parsedict| - parses a dictionary file and adds each word to the trie
|parsefile| - parses a file and checks each word against the trie
*/



#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include "./tries.h"
static int fd;

//tracks if the first word of the row has been read
short firstWord = 0;
//just a variable to check if the VERY FIRST word in the file has been read
short veryfirst = 0;
//this is actively counting the rows with every \n
unsigned int row = 1;
//this actively counts the col with the chars (should have named charcount)
unsigned int colcount = 1;
//this is holds col value for the beginning of a word
unsigned int colwrds = 1;
//this is the row value stored for as soon as the row switches
unsigned int activerow = 1;

//if a word is incorrect in ANY FILE, return EXIT_FAILURE
// if 1 that means to exit with failure, if 0 that means exit with success
short exitFailure = 0;

//pointer that saves the exitFailure if it is one
short saveExitMemory = 0;
short *saveExit = &saveExitMemory;

//have we hit the first char of the word?
short firstchar = 0;

//*colwp actively updates the col value for the beginning of a word
//why not just print the active col number? because it would be wrong
//our print statement prints AFTER a word is read. meaning it would get the col value after the word is read as well
//so we have a pointer pointing to the value of the beginning of words
unsigned int *colwp = &colwrds;
unsigned int *rowp = &activerow;

/**
 * 
*/
static char betterislower(char input){
    return input >= 'a' && input <= 'z';
}

static char betterisupper(char input){
    return input >= 'A' && input <= 'Z';
}
/**
 * A better isalpha implementation because the ctype one sucks. 
 * @arg input: a character we want to check, if it's an alpha character
 * @return 1 if character is alpha, 0 if not. 
*/
static char betterisalpha(char input){
    return betterislower(input) || betterisupper(input);
}
static char ISALLCAPITAL(char* input, int length){
    for(int i = 0; i < length; i++){
        if(!betterisupper(input[i]) && betterisalpha(input[i])) return 0;
    }
    return 1;
}
/**
 * This is to check for capitalization tolerances.
 * @arg a string from file
 * @arg a string from tree
 * @return 1 if string are same based on tolerances/ is a valid string
 * @return 0 if strings are not the same.
*/
static char tolerancecmp(char* a, char* b){
    // printf("a:%s\tb:%s\n",a,b);
    if(b == NULL){
        return 0;
    }
    unsigned int alength = strlen(a);
    unsigned int blength = strlen(b);
    if(alength != blength) return 0;
    for(int i = 0; i < alength; i++){
        if((betterislower(a[i]) && betterisupper(b[i])) && betterisalpha(a[i])){
             return 0;
        }
    }
    if(ISALLCAPITAL(a,alength)) return 1;
    for(int i = 1; i < alength; i++){
        if(betterisupper(a[i]) && betterislower(b[i]) && betterisalpha(a[i])) return 0;
    }
    return 1;

}
/**
 * This function is intended to take a word with non alpha characters on the side, and trim it
 * @arg word - a word containing the trailing characters in the begginning and end.
 * @return trimmed down string, NULL if string contains no alpha characters
*/
static char* chopword(char* word) {
    if (!strlen(word)) {
        return NULL;
    }

    //remove trailing stuff
    unsigned int R = strlen(word) - 1;
    unsigned int L = 0;
    while (!betterisalpha(word[L]) && L <= R) L++;
    while (!betterisalpha(word[R]) && L <= R) R--;

    if (L > R) {
        free(word);
        return NULL;
    }

    memmove(word, word + L, ((R - L) + 1));
    word[(R - L) + 1] = '\0';

    // dealing with '-'
    unsigned int length = strlen(word);
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == '-') {
            memmove((word + i), (word + i + 1), (length - i));
            length--; // Adjusting length since we removed a character
            i--; // Re-check the current index in case it's a hyphen
        }
    }
    return word;
}


/**
 * prints the string of size n
 * solely for troubleshooting
*/
void printstring(char* buffer, int size){
    for(int i = 0; i < size ; i++){
        printf("%c",buffer[i]);
    }
    printf("\n");
}

/**
 * This will parse a text file, and return a string, seperated by spaces
 * and hyphens to bridge words together. 
 * @return a string containing the word from a text file
*/
static char* getword() {
    int initsize = 8;
    char* mystring = (char*)malloc(sizeof(char) * initsize);
    char c[1]; 
    int reada = read(fd, c, 1);

    //this while loop is for when the first word of the row
    //is not at the beginning of the file. either by spaces or by newlines
    while ((c[0] == ' ' || c[0] == '\n') && reada != 0) {
        reada = read(fd, c, 1);
        //printf("character %c col %d\n",c[0],*colwp);
        
        colcount++;
        //print the colcount
        // printf("first word col: %d\n", colcount);
        
        if (c[0] == ' ' || c[0] == '\n') {
            if (c[0] == '\n') {
                row++;
                colcount = 0; //this could bite me in the butt later...
            }
            *colwp = colcount;
            firstWord = 0;
        }

    }


    int i = 0;

        if (firstchar == 0) {
            // printf("row: %d col: %d\n", row, colcount);

            *colwp = colcount;
            *rowp = row;
            firstchar = 1;
        }


    while (c[0] != '\n' && c[0] != ' ' && reada > 0) { 
        mystring[i++] = c[0];
        colcount++;

        // if (veryfirst == 0) {
        //     *colwp = colcount-1;
        //     veryfirst = 1;
        // }
        // printf("inside col: %d\n", colcount);

        
        //stores the value of the row for when characters are being read
        //we do not need to store the value of the row when the row is being reset.
        //only increment it
        //*rowp = row;

        //realloc the string if it is half full
        if (i >= (initsize / 2)) {
            char* newstring = realloc(mystring, initsize * 2);
            initsize *= 2;
            //free mystring if realloc fails
            if (newstring == NULL) {
                fprintf(stderr, "realloc failed\n");
                free(mystring);
                exit(EXIT_FAILURE);
            }

            //give mystring the newstring's allocation
            mystring = newstring;
        }


        // Read the next character
        reada = read(fd, c, 1);



        // Check if the next character is a newline or space
        if (c[0] == '\n' || c[0] == ' ') {
            firstWord = 1;
            
            firstchar = 0;
            if (c[0] == ' ') {
                colcount++;
            }


            //if the next character is a newline, increment the row and reset the column
            if (c[0] == '\n') {
                colcount = 1;
                row++;
            }

            // If it is, stop reading and break out of the loop
            break;
        }
    }

    //prints when we reach the end of a file
    if (i == 0) {
        //printf("---File Read Complete---\n");
        free(mystring);
        return NULL;
    }
    

    //for the case that we run out of memory somehow
    mystring[i++] = '\0';
    char* newstring = realloc(mystring, i);
    if (newstring == NULL) {
        fprintf(stderr, "unable to realloc string\n");
        free(mystring);
        exit(EXIT_FAILURE);
    }
    mystring = newstring;
    //printf("row: %d col: %d\n", row, col);
    return mystring;
}

//function that gets the word size for parsedict specifically
//originally the parsedict and parsefile were using the same getword function
//it was changed for column and                 
static char* getwordfordict() {
    int initsize = 8;
    char* mystring = (char*)malloc(sizeof(char) * initsize);
    char c[1]; 
    int reada = read(fd, c, 1);


    while ((c[0] == ' ' || c[0] == '\n') && reada != 0) {
    reada = read(fd, c, 1);
    }


    int i = 0;
    while (c[0] != '\n' && c[0] != ' ' && reada > 0) { 
        mystring[i++] = c[0];
        //realloc the string if it is half full
        if (i >= (initsize / 2)) {
            char* newstring = realloc(mystring, initsize * 2);
            initsize *= 2;
            //free mystring if realloc fails
            if (newstring == NULL) {
                fprintf(stderr, "realloc failed\n");
                free(mystring);
                exit(EXIT_FAILURE);
            }

            //give mystring the newstring's allocation
            mystring = newstring;
        }

        // printstring(mystring,i);
        // Read the next character
        reada = read(fd, c, 1);
        // Check if the next character is a newline or space
        if (c[0] == '\n' || c[0] == ' ') {
            // If it is, stop reading and break out of the loop
            break;
        }
    }
    if (i == 0) {
        //printf("---Dict Read Complete---\n");
        free(mystring);
        return NULL;
    }
    mystring[i++] = '\0';
    char* newstring = realloc(mystring, i);
    if (newstring == NULL) {
        fprintf(stderr, "unable to realloc string\n");
        free(mystring);
        exit(EXIT_FAILURE);
    }
    mystring = newstring;
    newstring = chopword(mystring);
    if(newstring == NULL){
        exit(EXIT_FAILURE);
    }
    return mystring;
}



//parses a dictionary file and adds each word to the trie
void parsedict(char* filepath){
    fd = open(filepath,O_RDONLY);
     if(fd == -1){

        perror("unable to open file\n");
        exit(EXIT_FAILURE);
    }

    char* mystring = getwordfordict();
    //int i = 0;

    while(mystring != NULL){
        // char* temp = chopword(mystring);
        // if(temp == NULL){
        //     exit(EXIT_FAILURE);
        // }
        // mystring = temp;
        mystring = chopword(mystring);
        put(mystring);
        free(mystring);
        mystring = getwordfordict();
    }
}

// parses a file and checks each word against the trie
short parsefile(char* filepath) {
    //we want to separate each word in the file
    //we will use the space as the delimiter
    //pass each word into the trie for comparison

    //column number gets reset
    row = 1;

    //open the file
    fd = open(filepath,O_RDONLY);

    //print error message and exit if the file is not found
    //this shouldn't even happen in the first place
    if(fd == -1){
        perror("unable to open file\n");
        exit(EXIT_FAILURE);
    }
 
    char* mystring = getword();
    //while we are not at the end of the file (0 is provided when the file ends)
    while(mystring != NULL){
            //printf("correct string: %s\n",get(mystring));
            char* treestring = get(mystring);
            
            if(!tolerancecmp(mystring,treestring)){
                printf("%s (row: %d,col: %d): %s\n",filepath, *rowp, *colwp, mystring);
                //printf("row: %d col: %d\n", row, colcount);
                exitFailure = 1;
            }
            free(mystring);   
        mystring = getword();
    }
    row = 1;
    colcount = 1;
    *rowp = row;
    *colwp = colcount;

    if (exitFailure) {
        *saveExit = 1; 
    }

    return exitFailure = *saveExit;
}



