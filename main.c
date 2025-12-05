#include <ncurses.h>
#include <stdlib.h>

#include "dat.h"

Screen *
screen_create(void)
{
	Screen *scr;
	int scrh, scrw;

	initscr();
	cbreak();
	noecho();
	curs_set(1);
	keypad(stdscr, TRUE);
    getmaxyx(stdscr, scrh, scrw);
	if ((scr = malloc(sizeof(Screen))) == nil)
		return nil;
	scr->h = scrh;
	scr->w = scrw;
	scr->cursy = scr->cursx = 0;
	scr->buf = nil;
	return scr;
}

void
screen_free(Screen *scr)
{
	buf_free(scr->buf);
	free(scr);
	endwin();
}

int
screen_movecurs(Screen *scr, int y, int x)
{
	return screen_movecursto(scr, scr->cursy + y,  scr->cursx + x);
}

int
screen_movecursto(Screen *scr, int y, int x)
{
	if (0 <= y && y < scr->h)
		scr->cursy = y;
	if (0 <= x && x < scr->w)
		scr->cursx = x;
	return move(scr->cursy, scr->cursx);
}

int
main(int argc, char *argv[])
{
	int c, quit;
	Screen *scr;

	if ((scr = screen_create()) == nil)
		sysfatal("cannot create screen");

	quit = 0;
	refresh();
	while (!quit) {
		c = getch();
		switch (c) {
		case 'h':
			screen_movecurs(scr, 0, -1);
			break;
		case 'l':
			screen_movecurs(scr, 0, 1);
			break;
		case 'k':
			screen_movecurs(scr, -1, 0);
			break;
		case 'j':
			screen_movecurs(scr, 1, 0);
			break;
		case 'Q':
			quit = 1;
			break;
		}
		if (c == KEY_BACKSPACE) {
		} else if (0x20 <= c && c <= 0x7e) {
		}
		refresh();
	}
	endwin();
	
	return 0;
}
