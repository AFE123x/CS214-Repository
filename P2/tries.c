#include "tries.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
struct Node{
  char* data;
  struct Node* left;
  struct Node* right;
  unsigned int height;  
};

struct Node* head = NULL;
unsigned int max(unsigned int A, unsigned int B){
    return (A > B) ? A : B;
}
unsigned int height(struct Node* curr){
    if(curr == NULL){
        return 0;
    }
    return curr->height;
}
int getbalance(struct Node* curr){
    if(curr == NULL){
        return 0;
    }
    return height(curr->left) - height(curr->right);
}
static struct Node* nodeconstructor(char* word){
    struct Node* toreturn = malloc(sizeof(struct Node));
    toreturn->data = malloc(strlen(word) + 1); // Allocate memory for the string data
    strcpy(toreturn->data, word); // Copy the string data
    toreturn->height = 1;
    toreturn->left = toreturn->right = NULL;
    // printf("Node created: %s\n",toreturn->data);
    return toreturn; // Don't forget to return the constructed node
}
static void tolowercase(char* a){
    for(int i = 0; a[i] != '\0'; i++){
        if(a[i] >= 'A' && a[i] < 'Z'){
            a[i] += 32;
        }
    }
}
int slaycmp(char* a, char* b){
    int length = (strlen(a) + 1);
    char* atemp = malloc(sizeof(char) * length);
    strcpy(atemp,a);
    length = strlen(b) + 1;
    char* btemp = malloc(sizeof(char) * length);
    strcpy(btemp,b);
    tolowercase(atemp);
    tolowercase(btemp);
    int decision = strcmp(atemp,btemp);
    free(atemp);
    free(btemp);
    return decision;
}
static struct Node* LRotate(struct Node* curr){
    struct Node* newhead = curr->right;
    struct Node* saveboi = newhead->left;
    newhead->left = curr;
    curr->right = saveboi;
    return newhead; // Return the new root of the subtree
}

static struct Node* RRotate(struct Node* curr){
    struct Node* newhead = curr->left;
    struct Node* saveboi = newhead->right;
    newhead->right = curr;
    curr->left = saveboi;
    return newhead; // Return the new root of the subtree
}
static char betterislower(char a){
    return a >= 'a' && a <= 'z';
}
/**
 * This function will perform a character OR operation
 * if upper | lower = lower
 * +---+---+---+
 * | U | L | L |
 * +---+---+---+
 * | L | U | L |
 * +---+---+---+
 * |L  | L | L |
 * +---+---+---+
 * | U | U | U |
 * +---+---+---+
 * @arg a tree string
 * @arg b input string
*/
static char bettertolower(char a){
    if((a >= 'A' && a <= 'Z')){
        return a + 32;
    }
    return a;
}
void charor(char* a, char* b){

    for(int i = 0; a[i] != '\0'; i++){
        if(betterislower(a[i]) || betterislower(b[i])){
            if(!betterislower(a[i])){
                a[i] = bettertolower(a[i]);
            }
        }
    }
}
static struct Node* put_helper(struct Node* curr, char* word){
    if(curr == NULL){
        return nodeconstructor(word);
    }
    int decision = slaycmp(word,curr->data);

    if(decision < 0){ //A < B
        curr->left = put_helper(curr->left,word);
    }
    else if(decision > 0){ //A > B
        curr->right = put_helper(curr->right,word);
    }
    else{
        // printf("%s | %s = ",curr->data,word);
        charor(curr->data,word);
        // printf(" %s\n",curr->data);
    }

    curr->height = max(height(curr->left),height(curr->right)) + 1;
    int balance = getbalance(curr);
    // printf("Balance is %d\n",balance);
    if(balance < -1 && getbalance(curr->right) < 0){ // RR case
        return LRotate(curr);
    }
    else if(balance < -1 && getbalance(curr->right) > 0){ // RL case
        curr->right = RRotate(curr->right);
        return LRotate(curr);
    }
    else if(balance > 1 && getbalance(curr->left) > 0){ // LL case
        return RRotate(curr);
    }
    else if(balance > 1 && getbalance(curr->left) < 0){ // LR case
        curr->left = LRotate(curr->left);
        return RRotate(curr);
    }

    //balancing tree
    //L 
    //R
    return curr;
}
static char* get_helper(struct Node* curr, char* word){
    if(curr == NULL){
        return NULL;
    }
    // printf("gettin word: %s\n",word);
    int decision = slaycmp(word,curr->data);

    if(decision < 0){ //A < B
        return get_helper(curr->left, word); // Call recursively on the left child
    }
    else if(decision > 0){ //A > B
        return get_helper(curr->right, word); // Call recursively on the right child
    }
    else{
        return curr->data;
    }
}

char* get(char* word){
    return get_helper(head,word);
}
char exists(char* word){
    return get(word) != NULL;
}
static void destroy_helper(struct Node* curr){
    if(curr == NULL){
        return;
    }
    destroy_helper(curr->left);
    destroy_helper(curr->right);
    free(curr->data);
    free(curr);
    
}
void destroy(){
    //printf("Destroying at %p\n",head);
    destroy_helper(head);
}
void put(char* word){
    head = put_helper(head,word);
}