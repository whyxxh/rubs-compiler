#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>

long get_file_size(FILE *f);
char *get_file_content(FILE *f);
FILE *open_file(int argc, char **argv);
void free_file(FILE *f, char *content);

#endif // FILEIO_H
