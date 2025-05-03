#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{ 
    if (argc < 2 ) {
        fprintf(stderr, "usage : ./rcc <file>\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("failled to open file");
    }    
    return 0;
}
