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
	scr->txt = nil;
	return scr;
}

void
screen_free(Screen *scr)
{
	text_free(scr->txt);
	free(scr);
	endwin();
}

int
screen_rendertext(Screen *scr)
{
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
	txt->file = nil;
	txt->buflinefrom = nil;
	txt->buflineto = nil;
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

File *
text_openfile(Text *txt, char *path, char *name)
{
	vlong ndisplay;
	Line *line;
	
	if ((txt->file = file_openfile(path, name)) == nil) 
		return nil;
	ndisplay = 1;
	txt->buflinefrom = txt->file->lines;
	for (line = txt->buflinefrom; ndisplay < txt->h && line->next != nil; line = line->next)
		ndisplay++;
	txt->buflineto = line;
	txt->buf = text_refreshbuf(txt);
	return txt->file;
}

Buffer *
text_refreshbuf(Text *txt)
{
	Buffer *newbuf;
	Line *line;

	/* TODO: Might be more efficient with a realloc of already existing buffer memory. */
	buf_free(txt->buf);
	if ((newbuf = buf_create(BUFFER_INIT_SIZE)) == nil)
		return nil;
	txt->buf = newbuf;
	for (line = txt->buflinefrom; line != nil; line = line->next) {
		buf_insert(txt->buf, line->bytes, line->len);
	}
	return txt->buf;
}

int
main(int argc, char *argv[])
{
	int c, quit;
	Screen *scr;


	if ((scr = screen_create()) == nil)
		sysfatal("cannot create screen");
	scr->txt = text_create(0, 0, scr->h, scr->w);
	text_openfile(scr->txt, "Makefile", "Makefile");
	screen_rendertext(scr);

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
	
	Line *line;

	line = scr->txt->buflinefrom;
	for (int i = 0;; i++) {
		printf("%d: %s", i, line->bytes);
		if (line == scr->txt->buflineto)
			break;
		line = line->next;
	}
	printf("scr->h = %d", scr->h);

	return 0;
}
