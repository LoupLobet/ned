#include "utf/utf.h"

#include "util.h"

#define BUFFER_INIT_SIZE 32

typedef struct Buffer Buffer;
typedef struct File File;
typedef struct Line Line;
typedef struct Screen Screen;
typedef struct Prompt Prompt;
typedef struct Text Text;

struct Buffer {
	char *bob;
	char *bog;
	char *eob;
	char *eog;
	vlong gapsize;
	vlong size;
	vlong len;
};

void	 buf_print(Buffer *); /* debug only */
vlong  buf_clean(Buffer *);
Buffer	*buf_create(unsigned int);
int	 buf_deleteafter(Buffer *, int);
int	 buf_deleteruneafter(Buffer *, int);
int	 buf_deletebefore(Buffer *, int);
int	 buf_deleterunebefore(Buffer *, int);
void	 buf_free(Buffer *);
int	 buf_insertafter(Buffer *, char *, int);
int	 buf_insertbefore(Buffer *, char *, int);
int	 buf_movebackwardn(Buffer *, int);
int	 buf_moveforwardn(Buffer *, int);
int	 buf_moverunebackwardn(Buffer *, int);
int	 buf_moveruneforwardn(Buffer *, int);
int  buf_moverunebackwardr(Buffer *, Rune);
int  buf_moveruneforwardr(Buffer *, Rune);
int  buf_moverunebackwardnr(Buffer *, int, Rune);
int  buf_moveruneforwardnr(Buffer *, int, Rune);
int  buf_nextrune(Buffer *, char **, Rune *);
int	 buf_resize(Buffer *);
char	*buf_tostr(Buffer *);

struct File {
	Line *lines;
	Line *lastline;
	char *name;	
};

void  file_free(File *);
File *file_openfile(char *, char *);

struct Line {
	vlong len;
	vlong cap;
	char *bytes;
	Line *next;	
	Line *prev;	
};

Line *line_appendc(Line *, char);
Line *line_create(void);
void line_free(Line *);
void line_freeall(Line *);

struct Screen {
	int h, w;
	Text *txt;
};

Screen *screen_create(void) ;
void screen_free(Screen *) ;
int screen_rendertext(Screen *);

struct Text{
	vlong bufcolfrom; /* bytes */
	vlong bufcolto; /* bytes */
	Line *buflinefrom;
	Line *buflineto;
	Buffer *buf;
	int curs;
	File *file;
	int h, w;
	int y, x;
};

Text *text_create(int, int, int, int);
void  text_free(Text *);
int   text_getcursrune(Text *, Rune *);
int   text_movecursbackwardn(Text *, int);
int   text_movecursforwardn(Text *, int);
File *text_openfile(Text *, char *, char *);
Buffer *text_refreshbuf(Text *);
