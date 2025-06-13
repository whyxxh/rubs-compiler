#include "../include/fileIO.h"
#include <stdlib.h>
#include <stdio.h>

long get_file_size(FILE *f);
char *get_file_content(FILE *f);
FILE *open_file(int argc, char **argv);

long get_file_size(FILE *f) 
{
    fseek(f, 0L, SEEK_END);
    long s = ftell(f);
    return s; 
}

char *get_file_content(FILE *f) 
{
    long s = get_file_size(f);
    rewind(f);
    char *content = malloc(s + 1);
    if (!content) {
        perror("failled content string allocation");
        return NULL;
    }
    size_t read = fread(content, sizeof(char), s, f);
    if (read != (long unsigned int)s) {
        perror("failled file reading");
        return NULL;
    }
    content[s] = '\0';
    return content;
}

FILE *open_file(int argc, char **argv) 
{
    if (argc < 2 ) {
        fprintf(stdout, "usage : %s <path_to_file>\n", argv[0]);
        return NULL;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("failled to open file");
        return NULL;
    }

    return f;
}

void free_file(FILE *f, char *content) 
{
    fclose(f);
    if (content)
        free(content);
}
