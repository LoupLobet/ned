#include "utf/utf.h"

#include "util.h"

#define BUFFER_INIT_SIZE 32

typedef struct Buffer Buffer;
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
Buffer	*buf_create(unsigned int);
int	 buf_deleteafter(Buffer *, int);
int	 buf_deleteruneafter(Buffer *, int);
int	 buf_deletebefore(Buffer *, int);
int	 buf_deleterunebefore(Buffer *, int);
void	 buf_free(Buffer *);
int	 buf_insert(Buffer *, char *, int);
int	 buf_movebackwardn(Buffer *, int);
int	 buf_moveforwardn(Buffer *, int);
int	 buf_moverunebackwardn(Buffer *, int);
int	 buf_moveruneforwardn(Buffer *, int);
int  buf_moverunebackwardr(Buffer *, Rune);
int  buf_moveruneforwardr(Buffer *, Rune);
int  buf_moverunebackwardnr(Buffer *, int, Rune);
int  buf_moveruneforwardnr(Buffer *, int, Rune);
int	 buf_resize(Buffer *);
char	*buf_tostr(Buffer *);

struct Screen {
	int h, w;
	Text *text;
	Text *prompt;
};

Screen *screen_create(void) ;
void screen_free(Screen *) ;

struct Text{
	int cursy, cursx;
	int y, x;
	int h, w;
	int addrfrom;
	Buffer *buf;
};

Text *text_create(int, int, int, int);
void text_free(Text *);
