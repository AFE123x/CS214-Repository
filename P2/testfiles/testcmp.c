#include<stdio.h>
#include<string.h>

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
        if(!betterisupper(input[i])) return 0;
    }
    return 1;
}
static char tolerancecmp(char* a, char* b){
    unsigned int alength = strlen(a);
    unsigned int blength = strlen(b);
    if(alength != blength) return 0;
    for(int i = 0; i < alength; i++){
        if(betterislower(a[i]) && betterisupper(b[i])){
            printf("%c and %c\n",a[i],b[i]);
             return 0;
        }
    }
    if(ISALLCAPITAL(a,alength)) return 1;
    for(int i = 1; i < alength; i++){
        if(betterisupper(a[i]) && betterislower(b[i])) return 0;
    }
    return 1;

}
int main(int argc, char** argv){
    printf("%s\n", tolerancecmp(argv[1],argv[2]) ? "same" : "not same");
}