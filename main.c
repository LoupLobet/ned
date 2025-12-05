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
	scr->text = nil;
	if ((scr->prompt = text_create(0, 0, scrh, scrw)) == nil) {
		free(scr);
		return nil;
	}
	return scr;
}

void
screen_free(Screen *scr)
{
	text_free(scr->prompt);
	text_free(scr->text);
	free(scr);
	endwin();
}

Text *
text_create(int y, int x, int h, int w)
{
	Text *txt;

	if ((txt = malloc(sizeof(Text))) == nil) {
		return nil;
	}
	txt->x = x;
	txt->y = y;
	txt->h = h;
	txt->w = w;
	txt->cursy = txt->cursx = 0;
	txt->addrfrom = 0;
	if ((txt->buf = buf_create(BUFFER_INIT_SIZE)) == nil) {
		free(txt);
		return nil;
	}
	return txt;
}

void
text_free(Text *txt)
{
	buf_free(txt->buf);
	free(txt);
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
			// screen_movecurs(scr, 0, -1);
			break;
		case 'l':
			// screen_movecurs(scr, 0, 1);
			break;
		case 'k':
			// screen_movecurs(scr, -1, 0);
			break;
		case 'j':
			// screen_movecurs(scr, 1, 0);
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
