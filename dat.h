#include "util.h"

#define BUFFER_INIT_SIZE 32

typedef struct Buffer Buffer;
typedef struct Screen Screen;

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
Buffer	*bufcreate(unsigned int);
int	 buf_deleteafter(Buffer *, int);
int	 buf_deleteruneafter(Buffer *, int);
int	 buf_deletebefore(Buffer *, int);
int	 buf_deleterunebefore(Buffer *, int);
void	 buf_free(Buffer *);
int	 buf_insert(Buffer *, char *, int);
int	 buf_movebackwards(Buffer *, int);
int	 buf_movecolumn(Buffer *, int);
int	 buf_moveforward(Buffer *, int);
int	 buf_moverunebackwards(Buffer *, int);
int	 buf_moveruneforward(Buffer *, int);
int	 buf_resize(Buffer *);
char	*buftostr(Buffer *);

struct Screen {
	int h, w;
	int cursy, cursx;
	Buffer *buf;
};

Screen *screen_create(void) ;
void screen_free(Screen *) ;
int screen_movecurs(Screen *, int, int);
int screen_movecursto(Screen *, int, int);
