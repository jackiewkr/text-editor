#ifndef FILE_H_
#define FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#define LINE_SZ (80)

/**
enum retval:
Stores return values from function
*/
enum retval {
        SUCCESS = 1,
	E_NO_MEM = 2,
	E_NAME_TOO_BIG = 3,
	E_UNKNOWN = 99
};

/** struct Line:
Stores data about a given line
*/
struct Line
{
	int l_sz;                //size of each line in characters
        char * l_data;         //string for each line
};

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

unsigned int init_line( struct Line * );

/*
unsigned int resize_file():
Doubles the linecount and reallocs memory for f_data. Returns E_NO_MEM if
realloc fails, else SUCCESS.
*/
unsigned int resize_file( struct File * );

void add_char_to_line( struct Line *, int, char );

void load_file( struct File *, char *, int );

void save_file( struct File *, char *, int );

#endif //FILE_H_
