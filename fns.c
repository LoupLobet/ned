#include <ncurses.h>

#include "utf/utf.h"

int
ncurses_printn(char *s, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        addch(s[i]);
    }
    return i;
}
