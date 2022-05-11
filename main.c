#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <ctype.h>
#include <string.h>

#include "file.h"

char * HL_keywords[] = { "auto", "break", "case", "continue", "default", "do",
	"else", "enum", "extern", "for", "goto", "if", "register", "return",
	"sizeof", "static", "struct", "switch", "typedef", "union", "volatile",
	"while", "NULL" };

char * HL_types[] = { "int", "long", "double", "float", "char", "unsigned",
	"signed", "void", "short", "auto", "const", "bool" };

/** struct Cursor:
Stores current position of curser (in lines and cols)
*/
struct Cursor
{
        int x, y;
};

/*
void start_ncurses():
Start ncurses mode and set some functions for the program.
*/
void start_ncurses()
{
        initscr();
	noecho(); /* No echo */
	keypad( stdscr, TRUE ); /* Arrow keys reading */
        cbreak(); /* No line buffering & no control characters */
	
	start_color();
	init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK );
}

/*
void end_ncurses():
Exit ncurses mode and reset anything that was changed by start_ncurse().
*/
void end_ncurses()
{
	echo();
	nocbreak();
        endwin();
}

/*
unsigned int parse_hl():
Given a word, return if it is a keyword, a type or neither.
0 for none, 1 for keyword, 2 for type
*/
unsigned int parse_hl( char * string )
{
        /* check if keyword */
	for ( int i = 0; i < 23; i++ ) {
                if ( strcmp( HL_keywords[i], string ) == 0 )
			return 1;
	}
	/* check if type */
	for ( int i = 0; i < 12; i++ ) {
                if ( strcmp( HL_types[i], string ) == 0 )
			return 2;
	}
	return 0;
};

void draw_line( struct Line l, int y, int x )
{
        mvprintw( y, x, "%s", l.line );
}

/*
void draw_file():
Draws a given file in the given section (from top_index to top_index+length or
file size depending on which is smaller).
*/
void draw_file( struct File f, int top_index, int length )
{
	int last_index = top_index + length;
	if ( top_index+length > f.sz )
		last_index = f.sz;
	
        for ( int i = top_index; i < last_index; i++ ) {
                draw_line( f.f_data[i], i - top_index, 3 );
	}
}

/*
void draw_bar():
Draws the bottom bar containing the current cursor position and the filename
//TODO show filename on bar
*/
void draw_bar( struct Cursor * c, int cols, int s_index )
{
        mvprintw( cols, 0, "%i,%i", c->x, c->y+s_index );
}

void draw_idx( struct File * f, int top_index, int length )
{
        int last_index = top_index + length;
	if ( top_index+length > f->sz )
		last_index = f->sz;

	for ( int i = top_index; i < last_index; i++ ) {
                mvprintw( i - top_index, 0, "%i", f->f_data[i].index );
	}
}

/* int main(): entrypoint of the program */
int main( int argc, char * argv[] )
{
	/* Create file and init data */
	struct File f;
	init_file( &f );
	
	if ( argc == 2 )
                load_file( &f, argv[1], strlen(argv[1]));

	/*  Create cursor */
	struct Cursor c;
	c.x = 0;
	c.y = 1;

	/* Start ncurses mode */
	start_ncurses();
        int cl_sz;
	int ch = KEY_BACKSPACE;
	int rows, cols;
	int s_index = 0;
	unsigned int done = 0;
	while ( !done ) {
                /* Main loop of program */
                getmaxyx(stdscr, rows, cols);

                switch (ch) {
                case KEY_LEFT:
			c.x--;
			break;
		case KEY_RIGHT:
			c.x++;
			break;
		case KEY_UP:
			c.y--;
			break;
		case KEY_DOWN:
			if (c.y != rows-2)
			        c.y++;
			break;
                case '\n':
			
			add_char_to_line( &f.f_data[c.y], c.x, ' ' );
			c.y++;
			c.x=0;
			break;
                case KEY_BACKSPACE:
			c.x--;
			add_char_to_line( &f.f_data[c.y], c.x, ' ' );
			break;
                case KEY_F(6):
			save_file( &f, argv[1], strlen(argv[1]));
			done = 1;
		default:
			if (c.x <= 79 && isprint(ch) ) {
			        add_char_to_line( &f.f_data[c.y + s_index], c.x, ch );
			        c.x++;
			}
			break;
		}
                cl_sz = f.f_data[c.y].sz;
		
                if (c.x > 80)
			c.x = 80;
		else if (c.x > cl_sz)
			c.x = cl_sz;
		if (c.x < 0 )
			c.x = 0;
		if (c.y < 0)
			c.y = 0;
		
		if (c.y == (f.sz-1) )
			resize_file( &f );
		if (c.y == (rows - 2) ) {
			clear();
			s_index++;
		}
		if (c.y == 0 && s_index != 0) {
			clear();
			s_index--;
		}
		#if 1	
		draw_idx( &f, s_index, rows - 1);
		draw_bar( &c, rows-1, s_index );
                draw_file( f , s_index, rows-1);
		move( c.y, c.x + 3 );
		#endif
		if (!done)
		        ch = getch();
		//clear();
		        refresh();
	}

	/* End ncurses mode */
	end_ncurses();

	
        return 0;
}
