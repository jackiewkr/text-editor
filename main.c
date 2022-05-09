#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <ctype.h>
#include <string.h>

#include "file.h"

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

void draw_line( struct Line * l, int y, int x )
{
        mvprintw( y, x, "%s", l->l_data );
}

void draw_file( struct File * f )
{
        for ( int i = 0; i < f->sz; i++ ) {
                draw_line( &f->f_data[i], i, 0 );
	}
}

void draw_bar( struct Cursor * c, int cols )
{
        mvprintw( cols, 0, "%i,%i", c->x, c->y );
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
			save_file( &f, "output.txt", 10);
			done = 1;
		default:
			if (c.x <= 79 && isprint(ch) ) {
			        add_char_to_line( &f.f_data[c.y], c.x, ch );
			        c.x++;
			}
			break;
		}
                cl_sz = f.f_data[c.y].l_sz;
		
                if (c.x > 80)
			c.x = 80;
		else if (c.x > cl_sz)
			c.x = cl_sz;
		if (c.x < 0 )
			c.x = 0;
		if (c.y < 1)
			c.y = 1;
		
		if (c.y == (f.sz-1) )
			resize_file( &f );

		draw_bar( &c, rows-1 );
                draw_file( &f );
		move( c.y, c.x );
		
		if (!done)
		        ch = getch();
		        clear();
		        refresh();
	}

	/* End ncurses mode */
	end_ncurses();

	
        return 0;
}
