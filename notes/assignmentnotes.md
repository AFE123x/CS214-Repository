# Spell checker

- Our program will read a dictionary file, and check the words in one text files to see if they're not in the dictionary file.

## Arguments

```c
./splchk arg_1 arg_*
```
- arg_1 is our dictionary file
- arg_* are the textfiles we want to check.


***Our dictionary file***

- this file has the correct words.
    - ilab has it in /usr/share/dict/words

***Text files***

- our text files.
    - Our text file can be broken down into lines
        - This means our file has a \n character.
        - This also means that the words are in one line.
    - We want to track the rows and collumns, i guess.

***trailing punctuation***: We ignore punctuations at the start and end of word.

***Hyphens***: words seperated by -. If we have:
- men-ny, we'd call it menny :-D.

***Capitalism***: We can have three capitals, regular, initial and all capitial
- HELLO is fine
- hello is okay
- Hello is chilling
- HeLlO is NONO D-:


## Directories

- If we pass a directory name, we gotta get all files that end with *.txt, and ignore hidden files starting with ., 


## Error reporting

- WE want to print the line and column number, yah.

sample output. 

```
    $ spchk my_dict file1 my_files
    file1 (35,8): foom
    my_files/bad.txt (1,1): Badd
    my_files/baz/bar.txt (8,19): almost-correkt
```

- error reporting for this:
    - print message if file couldn't be opened
    - exit with EXIT_SUCCESS if all files could be opened and contained no incorrect words.
        - otherwise, EXIT_FAILURE.


## Coding

1. Finding and opening all the specified files, including directory traversal
2. Reading the file and generating a sequence of position-annotated words
3. Checking whether a word is contained in the dictionary


- we must use read to read the dictionary and textfile.
    - nothing else
- write is suggested, but isn't required.