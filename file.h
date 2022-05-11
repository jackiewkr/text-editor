#ifndef FILE_H_
#define FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#define LINE_SZ (80)

/**
enum retval:
Stores return values from function
*/
enum retval {
        SUCCESS = 1,
	NEW_FILE = 2,
	E_NO_MEM = 90,
	E_UNKNOWN = 91
};

/** struct Line:
Stores data about a given line
*/
struct Line
{
	char * line;               //string representing line
	unsigned int sz;           //size of line in chars
	unsigned int index;        //absolute index of line
};

/*
unsigned int init_line():
Initializes a struct Line. Allocates some memory for l_data.
Returns E_NO_MEM if malloc fails, else SUCCESS.
*/
unsigned int init_line( struct Line *, unsigned int );

/*
void add_char_to_line():
Adds a char to a given struct Line at a given index. Basic checks are done
to ensure character isnt added out-of-bounds of the array
*/
void add_char_to_line( struct Line *, int, char );

/** struct File:
Stores data about a given file
*/
struct File
{
        struct Line * f_data;
	unsigned int sz;
};

/*
unsigned int init_file():
Initializes a struct File. Allocates some memory for f_data.
Returns E_NO_MEM if malloc fails, else SUCCESS.
*/
unsigned int init_file( struct File * );

/*
unsigned int resize_file():
Reallocates a given struct File. Returns E_NO_MEM if realloc() fails, else
SUCCESS.
*/
unsigned int resize_file( struct File * );

/*
unsigned int load_file():
Loads a given plaintext file string [buf] of size [sz]. if file doesnt exist,
create the file location. Returns NEW_FILE if file created, SUCCESS otherwise.
*/
unsigned int load_file( struct File *, char *, int );

/*
void save_file():
Copies data from struct File to a file on disk. Removes all non-human-readable
characters.
*/
void save_file( struct File *, char *, int );

#endif //FILE_H_
