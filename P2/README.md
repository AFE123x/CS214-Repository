# LexiconLuminator-A-Quintessential-Spellcraft-Endeavor

Project: Spelling checker

Due Date: 3/8/2024

Participating Students: Arun (afj277), Kareem(kkj47)

----

## Objective

Write a program spchk that reads a dictionary file and checks the words in one or more text files to
see if any are not listed in the dictionary. The first argument to spchk will be a path to a dictionary
file. Subsequent arguments will be paths to text files or directories.

## Testing

[parsetext.c](./P2/testfiles/parsetext.c) tests the functionality of the parsing function. 

[testcmp.c](./P2/testfiles/testcmp.c) tests the functionality of the function that manages the capitalization.

[trimword.c](./P2/testfiles/trimword.c) tests the functionality of removing trailing and previous characters.

The directory "dictdir" contains our testing dictionaries

The directory "filedir" contains our text files we used for testing against our dictionaries. 

### Input files

***dictionary files***

- We have varying dictionary files for different input types: varying spaces, same words with different spellings, etc. 

***txt files***

- filedir contains files that are used for parsing, testing different capitalizations, and words


## Design decisions

### Data Structure for storing
- We originally planned on using a ternary trie. unfortunately, I misread the material on it, and misintepreted the data structure. I thought I only needed one head node, then realized that I needed an array. I very much miss C++, and the powers of OOP, so we decided to go with the AVL tree.

- The AVL tree is a self balancing tree, and guarantees $log _2 n$, ensuring the difference in height between the first and second character is, at most, has a difference of one. 


### Row and Col implementation
- For checking where a word is in a given file we read it char by char and track whenever a word is hit, a space, or new line is hit. When a new line is hit the col is reset while the row increases in value. 




## How to run

```
cd P2
make
./spchk <dictionary location> <file directory> or <directory>
```

## Clean up files

```
cd P2
make clean
```