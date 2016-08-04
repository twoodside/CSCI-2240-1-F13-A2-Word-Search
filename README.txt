DESCRIPTION:
This program searches a Word Search Grid for a list of words and outputs the Word Search Grid with all letters removed save for those in the word list.

COMPILATION:
gcc twoodside_wordsearch.c

INPUT:
A text file with two pieces:
 1) An n*n grid of letters, each letter followed by a space:
    a b c 
    d e f 
    g h i 
 2) A list of new-line delimited words to find in the word search.
See Input.txt for a sample.

HOW TO RUN:
./a.out <Input.txt