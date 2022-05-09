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
        init_line( &f->f_data[0] );
	init_line( &f->f_data[1] );
	
	if ( f->f_data == NULL )
		return E_NO_MEM;
	return SUCCESS;
}

/*
unsigned int init_line():
Initializes a struct Line. Allocates some memory for l_data.
Returns E_NO_MEM if malloc fails, else SUCCESS.
*/
unsigned int init_line( struct Line * l )
{
	l->l_sz = 0;
        l->l_data = malloc( sizeof( char ) * LINE_SZ );
	if ( l->l_data == NULL )
		return E_NO_MEM;
	return SUCCESS;
}


/*
unsigned int resize_file():
Doubles the linecount and reallocs memory for f_data. Returns E_NO_MEM if
realloc fails, else SUCCESS.
*/
unsigned int resize_file( struct File * f )
{
	f->sz++;
        f->f_data = realloc( f->f_data, f->sz * sizeof( struct Line ) );

	init_line( &f->f_data[f->sz - 1] );

	if ( f->f_data == NULL )
		return E_NO_MEM;
	return SUCCESS;
}

void add_char_to_line( struct Line * l, int index, char ch )
{
        l->l_data[index] = ch;
	if (l->l_sz <= 79)
	        l->l_sz++;
}

void load_file( struct File * f, char * buf, int sz )
{
	if ( access( buf, F_OK) == 0 ) {
		FILE * fptr = fopen(buf, "r");
		ssize_t read;
		char * line = NULL;
		size_t len = 0;
		int index = 0;
		while ( ( read = getline( &line, &len, fptr ) ) != -1 ) {
			/* split into lines */
			resize_file( f );

			for (int i = 0; i < read; i++) {
				if (line[i] != '\n') {
					add_char_to_line( &f->f_data[index], i,
							  line[i] );
				}
			}
			index++;
		}

		fclose(fptr);
	} else {
                FILE * fptr = fopen(buf, "w");
		fclose(fptr);
	}
}

void save_file( struct File * f, char * buf, int sz )
{
        FILE * fptr = fopen(buf, "w");

	for (int i = 0; i < f->sz; i++) {
                for (int j = 0; j < f->f_data[i].l_sz; j++) {
			if (isprint(f->f_data[i].l_data[j]))
                                fputc(f->f_data[i].l_data[j], fptr);
		}
		fputc('\n', fptr);
	}

	fclose(fptr);
}
