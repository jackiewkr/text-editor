/**
file.c: Contains all functions to do with file operations, both internal and
external.
*/

#include "file.h"

/*
unsigned int init_file():
Initializes a struct File. Allocates some memory for f_data.
Returns E_NO_MEM if malloc fails, else SUCCESS.
*/
unsigned int init_file( struct File * f )
{
	/* allocate 2 lines to start with */
	f->f_data = malloc( sizeof( struct Line ) * 2 );
	f->sz = 2;
        init_line( &f->f_data[0], 0 );
	init_line( &f->f_data[1], 1 );
	
	if ( f->f_data == NULL )
		return E_NO_MEM;
	return SUCCESS;
}

/*
unsigned int init_line():
Initializes a struct Line. Allocates some memory for l_data.
Returns E_NO_MEM if malloc fails, else SUCCESS.
*/
unsigned int init_line( struct Line * l, unsigned int index )
{
	l->index = index;
	l->sz = 0;
        l->line = malloc( sizeof( char ) * LINE_SZ );
	if ( l->line == NULL )
		return E_NO_MEM;
	return SUCCESS;
}


/*
unsigned int resize_file():
Reallocates a given struct File. Returns E_NO_MEM if realloc() fails, else
SUCCESS.
*/
unsigned int resize_file( struct File * f )
{
	f->sz++;
        f->f_data = realloc( f->f_data, f->sz * sizeof( struct Line ) );
        init_line( &f->f_data[f->sz - 1], f->sz-1 );
	if ( f->f_data == NULL )
		return E_NO_MEM;
	return SUCCESS;
}

/*
void add_char_to_line():
Adds a char to a given struct Line at a given index. Basic checks are done
to ensure character isnt added out-of-bounds of the array
*/
void add_char_to_line( struct Line * l, int index, char ch )
{
	l->line[index] = ch;
	if (l->sz <= 79)
		l->sz++;
}

static void recheck_line( struct Line * l )
{
        for (int i = 0; i < l->sz; i++) {
                if ( l->line[i] == '\0' )
			l->line[i] = ' ';
	}
}

/*
void add_string_to_line():
Adds a string to a given struct Line at given starting index. Passes each char
to add_char_to_line().
*/
static void add_string_to_line( struct Line * l, int s_index, char * str,
				int sz )
{
	int i = 0;
	int offset = 0;
        while ( i < sz ) {
		if ( str[i] == '\t' ) {
			add_string_to_line( l, (s_index + i), "        ", 8 );
			offset += 7;
		}
		else {
			if ( isprint(str[i]) ) {
				add_char_to_line( l, (s_index + offset),
						  str[i] );
			}
		}
		i++;
		offset++;
	}

	//recheck_line( l );
}

/*
unsigned int load_file():
Loads a given plaintext file string [buf] of size [sz]. if file doesnt exist,
create the file location. Returns NEW_FILE if file created, SUCCESS otherwise.
*/
unsigned int load_file( struct File * f, char * buf, int sz )
{
	/* Check if file exists */
	if ( access( buf, F_OK) == 0 ) {                         //exists
		FILE * fptr = fopen( buf, "r" );
		
		ssize_t read;                 //stores length of line from file
		char * line = NULL;           //stores line from file
		
		size_t len = 0;
		int index = 0;
		/* while there are still lines to read, do this */
		while ( ( read = getline( &line, &len, fptr ) ) != -1 ) {
			/* Resize file to accommodate new line */
			resize_file( f );

			/* Adds each character to the file if it is readable */
			add_string_to_line( &f->f_data[index], 0, line, read );
			index++;
		}

		fclose(fptr);
		return SUCCESS;
	} else {                                                 //not exists
                /* Create file then exit */
                FILE * fptr = fopen(buf, "w");
		fclose(fptr);
		return NEW_FILE;
	}
}

/*
void save_file():
Copies data from struct File to a file on disk. Removes all non-human-readable
characters.
*/
void save_file( struct File * f, char * buf, int sz )
{
        FILE * fptr = fopen(buf, "w");
        /* for each line and char in line, put char in file if it is HR */
	for ( int l_idx = 0; l_idx < f->sz; l_idx++ ) {
                for ( int ch_idx = 0; ch_idx < f->f_data[l_idx].sz; ch_idx++ ){
			if ( isprint( f->f_data[l_idx].line[ch_idx] ) )
                                fputc( f->f_data[l_idx].line[ch_idx], fptr);
		}
		/* add in newline to separate lines */
		fputc('\n', fptr);
	}

	fclose(fptr);
}
