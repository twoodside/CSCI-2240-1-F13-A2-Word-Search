#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** readInGrid( int* );
char**readInWords(int,int*);
int charPos(char*,char,int);
char* thin(char*,int);
void bubbleSort(char**,int);
int compare(char*,char*);
int** findWords(char**,char**,int,int);
void printFinalGrid(char**,int**,int);
int inWordList(char,int,char**,int*,int*);
int wordInWordList(char*,char**,int,int);

int main(){
    char** grid; /* n*(n*2+1) 2d array of characters contained in the grid. Spaces are included. I.e.
                 a b c 
                 d e f
                 g h i */
    char** wordList; /*List of words to find in the wordsearch. Case sensitive. Max length of n I.e.
                     abc
                     heb
                     iea
                     aei*/
    int** binaryList; /* n*(n*2+1) 2d array of booleans for whether to print the character in the same location in grid. Spaces are always marked as 0, characters may be 0 or 1. I.e
                      101010
                      001000
                      001010*/

            /*The final printing for the example grid will be:
            *a b c *
            *  e   *
            *  h i *
            *Asterisks show delimitation, not actual printed characters*/
    int gridSize=1; /*In the grid's dimensions above, n*/
    int wordListLength=1; /*Number of words to find.*/

    grid=readInGrid(&gridSize);

    wordList=readInWords(gridSize,&wordListLength);

    binaryList=findWords(grid,wordList,gridSize,wordListLength);

    printFinalGrid(grid,binaryList,gridSize);
}

/*Takes a string, returns a substring of size size<string size.*/
/*Probably works like realloc, but realloc didn't work for me when I tried it.*/
char* thin(char* str, int size){
    char* r=(char*)malloc(sizeof(char)*(size));
    int i;

    for (i=0;i<size;i++){
        r[i]=str[i];
    }
    r[size-1]='\0';

    return r;
}

/*Returns the position of char toFind in string line.*/
int charPos(char* line,char toFind,int size){
    int r=-1;
    int i;
    for (i=0;i<size;i++){
        if (line[i]==toFind){
            r=i;
            break;
        }
    }

    return r;
}
/*It's a bubble sort. Sorts the strings in toSort in alphabetical order. Seperates upper/lower case.*/
void bubbleSort(char** toSort,int size){
    int i,j;
    for (i=0;i<(size-1);i++){
        for (j=0;j<size-i-1;j++){
            char* swap;
            if (compare(toSort[j],toSort[j+1])==1){ /*If a word is alphabetically later than the next index, swap them.*/
                swap=toSort[j];
                toSort[j]=toSort[j+1];
                toSort[j+1]=swap;
            }
        }
    }
}
/*Compares two strings. Returns 1 if a is alphabetically later than b, 0 if they're the same word, and -1 if a is earlier.*/
int compare(char* a,char* b){
    int i=0;
    while (a[i]!='\0' && b[i]!='\0' && a[i]==b[i]){
        i++;
    }
    if (a[i]>b[i]) return 1;
    if (a[i]==b[i]) return 0;
    if (a[i]<b[i]) return -1;
}
/*Binary search to see if toFind is in wordList.*/
int wordInWordList(char* toFind, char** wordList,int min,int max){
     int comp=(min+max)/2; /*Index to compare to.*/
     int lteqgt; /*Less than, equal to, greater than.*/

     if (min>max){
         return 0;
     }
     
     lteqgt=compare(toFind,wordList[comp]);
     if (lteqgt==0) return 1;
     if (lteqgt==1) return wordInWordList(toFind,wordList,comp+1,max);
     if (lteqgt==-1) return wordInWordList(toFind,wordList,min,comp-1);



}
/*Looks to see if character toFind is in a certain position in a subset of the word list. I.e.
inWordList(a,2,wordList,2,4);
will look for the letter a in the third position of any of the words in wordList[2] through wordList[4].

The function will also change min to be the first location after the current min where toFind is found,
and will change max to be the final position in the subset where it's found. So if the word list is:
abcdef
Haha
Hello
Hello World
Hello there world!
Help me!
xyz

and the call is:
inWordList(e,1,wordList,1,5);

The function would return 1 and change min to 2 and max to 4 in the calling function.

If the call were instead
inWordList(w,0,wordList,0,wordListLength);

The function would return 0 and min and max in the calling function would be unchanged.
*/

int inWordList(char toFind, int pos,char** wordList, int* min, int* max){
    int i,r=0;
    for (i=*min;i<=*max;i++){ /*Loop through the wordList*/
        if (wordList[i][pos]==toFind){ /*If you find the char in the position*/
            *min=i; /*Set the minimum to the first location in wordList the char was found*/
            i++;
            while (i<=*max && wordList[i][pos]==toFind){ /*Find the last valid position in wordList*/
                i++;
            }
            *max=i-1; /*Set the maximum*/
            r=1; /*Return true*/
            break; /*Stop looping.*/
        }
    }
    return r;
}
/*Reads in the grid from standard input. Returns the grid.*/
/*gridSizePos-An int var in the calling function representing the size of the grid. Will be changed by the function.*/
char** readInGrid(int* gridSizePos){
    char* findSize=(char*)calloc( 50*2+1,sizeof(char) ); /*Clear some space for a temporary line to find the size of the grid.*/
    char** r;
    int i;
    int gridSize;
    char c;

    fgets(findSize,50,stdin); /*Reads in the first line of the grid*/

    gridSize=charPos(findSize,'\0',50); /*Finds how long the line actually is, depending on where the null char is.*/
    gridSize=--gridSize/2; /*Sets the gridSize so that the line length is gridSize*2+1 and the number of lines is gridSize.*/

    r=(char**)malloc(sizeof(char*)*(gridSize)); /*Allocates the appropriate amount of space for the gridSize.*/
    r[0]=thin(findSize,gridSize*2+1); /*Saves the first line.*/
    for (i=1;i<gridSize;i++){
        /*Read in a line of gridSize*2+1 chars.*/
        r[i]=(char*)malloc(sizeof(char)*(gridSize*2+1)); 
        fgets(r[i],gridSize*2+1,stdin);
        while((c = getchar()) != '\n' && c != EOF); /*Clear the line*/
    }

    *gridSizePos=gridSize; /*Change the gridSize in the calling function*/
    return r;
}

/*Reads in the wordList from standard input. Returns the wordList.*/
/*length-the longest any word to be read can be.
  lengthPos-An int var in the calling function representing the amount of words in the wordList. Will be changed by the function.*/
char** readInWords(int length,int* lengthPos){
    char** r=(char**)malloc(sizeof(char*)*length); /*Free up space. Initially, the max number of words will be gridSize, but the function will dynamically allocate more space as needed.*/
    int currentSize=length; /*Remember how much space the current list has available, in case there are too many words*/
    int i=-1;

    do{
        if (++i>=currentSize){ /*If there are too many words, double the available space for the wordList*/
            currentSize*=2;
            r=realloc(r, sizeof(char*)*currentSize);
        }

        r[i]=(char*)malloc(sizeof(char)*(length+1));
    }while (scanf("%s",r[i])!=EOF); /*Read until end of file*/

    r=realloc(r,sizeof(char*)*i); /*Trim up the list so it doesn't take up a bunch of extra space in memory that it's not using.*/
    bubbleSort(r,i); /*Sort the list*/
    *lengthPos=i; /*Change wordListLength in the calling function*/

    return r; /*Return the wordList*/
}

/*Finds the words in the wordList within the grid*/
/*Returns a grid of booleans*/
int** findWords(char** grid, char** wordList, int gridSize, int wordListLength){
    int** r=(int**)malloc(gridSize*sizeof(int*));
    int i,j;
    int min=0;
    int max=wordListLength-1;

    /*Allocates a 2d grid, and sets all values to 0.*/
    for (i=0;i<gridSize;i++){
        r[i]=(int*)malloc(sizeof(int)*(gridSize*2+1));
        for (j=0;j<gridSize*2+1;j++){
            r[i][j]=0;
        }

    }

    /*Loops through each position in the grid. When it finds a char that corresponds to a char in the first position of at least one of the words in wordList,
    the function will look in each of the eight adjacent locations. If the location has a letter that is the second letter of one of the words that start with the
    first letter, then the function will continue looking in that direction until it finds a letter that doesn't fit into any of the words. Then it goes backwards,
    until it finds a word in the wordList, at which point it sets all the corresponding positions in the return grid to 1/true. If it never actually found a word,
    the function won't set anything to 1 for that direction.
    */
    for (i=0;i<gridSize;i++){
        for (j=0;j<gridSize*2+1;j+=2){
            
            int a,b,preMin,preMax,ti,tj,k;
            char* holding;
            if (inWordList(grid[i][j],0,wordList,&min,&max)){ /*If grid[i][j] is in the first position of one of the words in wordList*/
                
                for (a=-1;a<=1;a++){ /*Loop through the eight adjacent positions.*/
                    for (b=-2;b<=2;b+=2){
                        
                        if ( !(a+i<0 || a+i>=gridSize || b+j<0 || b+j>=gridSize*2+1 || a+b==0) ){ /*If the position isn't outside the grid*/
                            preMin=min;
                            preMax=max;
                            ti=i+a; /*Temporary looping variables*/
                            tj=j+b;
                            k=1;
                            holding=(char*)malloc(sizeof(char)*(gridSize+1)); /*Holds the word as it's found*/
                            holding[0]=grid[i][j];
                            holding[1]='\0';

                            while ( !(ti<0 || ti>=gridSize || tj<0 || tj>=gridSize*2+1) && inWordList(grid[ti][tj],k,wordList,&min,&max) && k<gridSize){/*If the next letter is a valid next letter*/
                                
                                /*Add the letter to the word holder*/
                                holding[k]=grid[ti][tj];
                                holding[k+1]='\0';
                                /*Then continue along in that direction*/
                                ti+=a; 
                                tj+=b;
                                k++;
                            }
                            
                            min=preMin;
                            max=preMax;
                            ti-=a;
                            tj-=b;
                            /*Goes backwards until a word is found or you're back to the start*/
                            while(ti!=i-a || tj!=j-b){
                                holding[k]='\0'; /*Remove the last letter*/
                                if (wordInWordList(holding,wordList,min,max)){
                                    while (ti!=i-a || tj!=j-b){ /*If a word is found, set all the positions to be 1 until back at the start*/
                                        r[ti][tj]=1;
                                        ti-=a;
                                        tj-=b;
                                    }
                                    break;
                                }
                                k--; /*Decrement everything*/
                                ti-=a;
                                tj-=b;
                            }
                                    
                        }
                    }
                }
            }
            min=0; /*Reset the min and max*/
            max=wordListLength-1;
        }
        
    }

    return r;
}

/*Print the final grid, with only the characters in the found words printed.*/
void printFinalGrid(char** grid, int** binaryList,int gridSize){
    /*For each character in the grid, if the binaryList boolean at the same position is 1, print the character, otherwise print a space.*/
    int i,j;
    for (i=0;i<gridSize;i++){
        for (j=0;j<gridSize*2-1;j++){
            if (binaryList[i][j])
                printf("%c",grid[i][j]);
            else
                printf(" ");
        }
        printf(" \n");
    }
}
