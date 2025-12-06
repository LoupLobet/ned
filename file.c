#include<stdlib.h>

#include "dat.h"
#include "util.h"

#define LINE_CAP_INCR 32

void
file_free(File *file)
{
    free(file->name);
    line_freeall(file->lines);
}

File *
file_openfile(char *path, char *name)
{
    File *file;
    FILE *fp;
    Line *line;
    char c;

    if ((fp = fopen(path, "r")) == nil)
        return nil;
    if ((file = malloc(sizeof(File))) == nil) {
        fclose(fp);
        return nil;
    }
    if ((file->lines = line_create()) == nil) {
        fclose(fp);
        free(file);
        return nil;
    }
    /* Load the file as lines. */
    line = file->lines;
    file->lastline = file->lines;
    while (fread(&c, 1, 1, fp)) {
        line_appendc(line, c);
        if (c == '\n') {
            if ((line->next = line_create()) == nil) {
                line_freeall(file->lines);
                fclose(fp);
                free(file);
                return nil;
            }
            line->next->prev = line;
            file->lastline = line;
            line = line->next;
        }
    }
    fclose(fp);
    if ((file->name = strdup(name)) == nil) {
        line_freeall(file->lines);
        fclose(fp);
        free(file);
        return nil;
    }
    return file;
}

Line *
line_appendc(Line *line, char c)
{
    if (line->len == line->cap) {
        /*
         * Make lines grow by constant value (instead of multiplicative factor)
         * to minimize memory footprint. A greater iteration per line ratio only
         * impacts file opening time.
         */
        line->cap += LINE_CAP_INCR;
        /*
         * We bluntly realloc since we never want to keep a line that cannot be
         * loaded from the begining to the end.
         */
        if ((line->bytes = realloc(line->bytes, line->cap)) == nil)
            return nil;
    }
    line->bytes[line->len++] = c;
    return line;
}

Line *
line_create(void)
{
    Line *line;

    if ((line = malloc(sizeof(Line))) == nil)
        return nil;
    line->cap = 32;
    line->len = 0;
    if ((line->bytes = malloc(line->cap)) == nil) {
        free(line);
        return nil;
    }
    line->prev = nil;
    line->next = nil;
    return line;
}

void
line_free(Line *line)
{
    free(line->bytes);
    free(line);
}

void
line_freeall(Line *line)
{
    Line *next;

    next = line->next;
    free(line->bytes);
    free(line);
    if (next != nil)
        line_freeall(next);
}
