# implementation ideas

- File structure, ideally having:
    - tries structure for search
        - actual structure to store dictionary
    - main
        - client interface
    - maybe an abstraction over read()
        - maybe include read_word to return a word?



## Tries

we want the following Public API:
```c
void put(char* word); //putting string in tries
char* get(char* word); //returns word if success, NULL if fail?
void destroy(); //clean our tries, free memory.
char exists(char* word); //return 1 if true, 0 if false.
```

and the following private API:

```c
struct Node* put(struct Node* curr, char* word, int index, int length); //actual put method. include string.h, use strlen.
char* tolowercase(char* buffer); //turn everything to lowercase. put in splchk instead.
```


### Implementation ideas

our struct
```c
struct Node{
private char c; //character in node
char iscomplete; //if it's end of word
struct Node* left //left child
struct Node* mid //middle child
struct Node* right; // right child
}
```


#### Put method

```java
public class TST<Value>
{
private Node root;

private class Node
{ /* see previous slide */ }
public void put(String key, Value val)
{ root = put(root, key, val, 0); }
private Node put(Node x, String key, Value val, int d){
        char c = key.charAt(d);
        if (x == null) { x = new Node(); x.c = c; }
        if (c < x.c) x.left = put(x.left, key, val, d);
        else if (c > x.c) x.right = put(x.right, key, val, d);
        else if (d < key.length() - 1) x.mid = put(x.mid, key, val, d+1);
        else x.val = val;
        return x;
    }
}
```


#### Get method

```java
public boolean contains(String key)
{ return get(key) != null; }
public Value get(String key){
    Node x = get(root, key, 0);
    if (x == null) return null;
    return x.val;
}
private Node get(Node x, String key, int d){
    if (x == null) return null;
    char c = key.charAt(d);
    if (c < x.c) return get(x.left, key, d);
    else if (c > x.c) return get(x.right, key, d);
    else if (d < key.length() - 1) return get(x.mid, key, d+1);
    else return x;
}
```



***TO DO: FIGURE OUT HOW TO AUTOCORRECT?***


## Abstraction over read

- let's say we want to read one word at a time, how to do this?
    - read_word() function?