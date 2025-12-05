#include <stdlib.h>
#include <string.h>

#include "dat.h"
#include "utf/utf.h"

void
buf_print(Buffer *buf)
{
	char *c;
	int gap;

	gap = 0;
	c = buf->bob;
	for (c = buf->bob; c <= buf->eob; c++) {
		if (c == buf->bog)
			gap = 1;
		if (gap)
			putchar('_');
		else
			putchar(*c);
		if (c == buf->eog)
			gap = 0;
	}
}

Buffer *
buf_create(unsigned int size)
{
	Buffer *buf;
	
	if ((buf = malloc(sizeof(Buffer))) == NULL)
		return NULL;
	if ((buf->bob = malloc(size)) == NULL) {
		free(buf);
		return NULL;
	}
	buf->len = 0;
	buf->size = size;
	buf->gapsize = buf->size;
	buf->eob = buf->bob + buf->size - 1;
	buf->bog = buf->bob;
	buf->eog = buf->eob;
	return buf;
}

int
buf_movebackwardn(Buffer *buf, int n)
{
	int moved;

	moved = 0;
	while (moved != n) {
		if (buf->bog == buf->bob)
			return moved;
		*buf->eog = *(buf->bog - 1);
		buf->bog--;
		buf->eog--;
		moved++;
	}
	return moved;
}

int
buf_moveforwardn(Buffer *buf, int n)
{
	int moved;

	moved = 0;
	while (moved != n) {
		if (buf->eog == buf->eob)
			return moved;
		*buf->bog = *(buf->eog + 1);
		buf->bog++;
		buf->eog++;
		moved++;
	}
	return moved;
}

int
buf_moverunebackwardn(Buffer *buf, int n)
{
	Rune r;
	int moved;
	int i, runelen;

	moved = 0;
	while (moved != n) {
		if (buf->bog == buf->bob)
			return moved;
		/* find the last rune before the gap */
		for (i = 0; i < 4; i++) {
			if (buf->bog - i == buf->bob)
				return moved;
			runelen = chartorune(&r, buf->bog - i - 1);
			if (r == Runeerror)
				continue;
			memcpy(buf->eog - runelen + 1, buf->bog - runelen, runelen);
			buf->bog -= runelen;
			buf->eog -= runelen;
			moved++;
			break;
		}
		if (r == Runeerror) /* invalid utf8 codepoint */
			return moved;
	}
	return moved;
}

int
buf_moveruneforwardn(Buffer *buf, int n)
{
	Rune r;
	int moved;
	int runelen;

	moved = 0;
	while (moved != n) {
		if (buf->eog == buf->eob)
			return moved;
		runelen = chartorune(&r, buf->eog + 1);
		if (r == Runeerror)
			return moved;
		memcpy(buf->bog, buf->eog + 1, runelen);
		buf->bog += runelen;
		buf->eog += runelen;
		moved++;
	}
	return moved;
}

int
buf_moverunebackwardnr(Buffer *buf, int n, Rune until)
{
	Rune r;
	int moved;
	int i, runelen;

	moved = 0;
	if (!n)
		return 0;
	do {
		if (buf->bog == buf->bob)
			return moved;
		/* find the last rune before the gap */
		for (i = 0; i < 4; i++) {
			if (buf->bog - i == buf->bob)
				return moved;
			runelen = chartorune(&r, buf->bog - i - 1);
			if (r == Runeerror)
				continue;
			memcpy(buf->eog - runelen + 1, buf->bog - runelen, runelen);
			buf->bog -= runelen;
			buf->eog -= runelen;
			moved++;
			break;
		}
		if (r == Runeerror) /* invalid utf8 codepoint */
			return moved;
	} while (moved != n && r != until);
	return moved;
}

int
buf_moveruneforwardnr(Buffer *buf, int n, Rune until)
{
	Rune r;
	int moved;
	int runelen;

	moved = 0;
	if (!n)
		return 0;
	do {
		if (buf->eog == buf->eob)
			return moved;
		runelen = chartorune(&r, buf->eog + 1);
		if (r == Runeerror)
			return moved;
		memcpy(buf->bog, buf->eog + 1, runelen);
		buf->bog += runelen;
		buf->eog += runelen;
		moved++;
	} while(moved != n && r != until);
	return moved;
}

int
buf_moverunebackwardr(Buffer *buf, Rune until)
{
	Rune r;
	int moved;
	int i, runelen;

	moved = 0;
	do {
		if (buf->bog == buf->bob)
			return moved;
		/* find the last rune before the gap */
		for (i = 0; i < 4; i++) {
			if (buf->bog - i == buf->bob)
				return moved;
			runelen = chartorune(&r, buf->bog - i - 1);
			if (r == Runeerror)
				continue;
			memcpy(buf->eog - runelen + 1, buf->bog - runelen, runelen);
			buf->bog -= runelen;
			buf->eog -= runelen;
			moved++;
			break;
		}
		if (r == Runeerror) /* invalid utf8 codepoint */
			return moved;
	} while (r != until);
	return moved;
}

int
buf_moveruneforwardr(Buffer *buf, Rune until)
{
	Rune r;
	int moved;
	int runelen;

	moved = 0;
	do {
		if (buf->eog == buf->eob)
			return moved;
		runelen = chartorune(&r, buf->eog + 1);
		if (r == Runeerror)
			return moved;
		memcpy(buf->bog, buf->eog + 1, runelen);
		buf->bog += runelen;
		buf->eog += runelen;
		moved++;
	} while(r != until);
	return moved;
}

int
buf_insert(Buffer *buf, char *s, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		*buf->bog = s[i];
		buf->bog++;
		buf->gapsize--;
		buf->len++;
		if (buf->gapsize <= 1) {
			if (!buf_resize(buf))
				return i;
		}
	}
	return i;
}

int
buf_deleteafter(Buffer *buf, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		if (buf->eog == buf->eob)
			return i;
		buf->eog++;
		buf->gapsize++;
		buf->len--;
	}
	return i;
}

int
buf_deleteruneafter(Buffer *buf, int n)
{
	int i, runelen;
	Rune r;

	for (i = 0; i < n; i++) {
		if (buf->eog == buf->eob)
			return i;
		runelen = chartorune(&r, buf->eog + 1);
		if (r == Runeerror)
			return i;
		buf->eog += runelen;
		buf->gapsize += runelen;
		buf->len -= runelen;
	}
	return i;
}

int
buf_deletebefore(Buffer *buf, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		if (buf->bog == buf->bob)
			return i;
		buf->bog--;
		buf->gapsize++;
		buf->len--;
	}
	return i;
}

int
buf_deleterunebefore(Buffer *buf, int n)
{
	int i, j, runelen;
	Rune r;

	for (i = 0; i < n; i++) {
		if (buf->bog == buf->bob)
			return i;
		/* find the last rune before the gap */
		for (j = 0; j < 4; j++) {
			if (buf->bog - j == buf->bob)
				return i;
			runelen = chartorune(&r, buf->bog - 1 - j);
			if (r == Runeerror)
				continue;
			buf->bog -= runelen;
			buf->gapsize += runelen;
			buf->len -= runelen;
			break;
		}
		if (r == Runeerror) /* invalid utf8 codepoint */
			return i;
	}
	return i;
}

void
buf_free(Buffer *buf)
{
	free(buf->bob);
	free(buf);
}

int
buf_resize(Buffer *buf)
{
	char *new;
	unsigned int leftsize, newsize, rightsize;
	unsigned int gapsize;

	newsize = (buf->size < 1 ? 2 : buf->size * 2);
	leftsize = buf->bog - buf->bob;
	rightsize = buf->eob - buf->eog;
	if ((new = realloc(buf->bob, newsize)) == NULL)
		return 0;
	gapsize = newsize - (buf->size - buf->gapsize);
	if (buf->eog != buf->eob)
		memcpy(new + leftsize + gapsize, new + leftsize + buf->gapsize, rightsize);
	buf->bob = new;
	buf->bog = buf->bob + leftsize;
	buf->eog = buf->bog + gapsize - 1;
	buf->eob = new + newsize - 1;
	buf->size = newsize;
	buf->gapsize = gapsize;
	/* buf->len  = buf->len */
	return newsize;
}

char *
buf_tostr(Buffer *buf)
{
	char *p, *q, *s;

	if ((s = calloc(buf->len + 1, 1)) == NULL)
		return NULL;
	q = s;
	for (p = buf->bob; p <= buf->eob; p++) {
		if (p == buf->bog) {
			if (buf->eog == buf->eob)
				break;
			p = buf->eog + 1;
		}
		*q = *p;
		q++;
	}
	return s;
}
