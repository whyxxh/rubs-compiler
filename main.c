#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long get_file_size(FILE *f) {
	fseek(f, 0L, SEEK_END);
	long s = ftell(f);
	return s;
}

char *get_file_content(FILE *f) 
{
	long s = get_file_size(f);
	rewind(f);
	char *content = malloc(s + 1);
	fread(content, sizeof(char), s, f);
	content[s] = '\0';
	return content;    
}

FILE *open_file(int argc, char **argv) 
{
	if (argc < 2 ) {
		fprintf(stdout, "usage : %s <file>\n", argv[0]);
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
	free(content);
}

int main(int argc, char **argv) 
{ 
	FILE *f = open_file(argc, argv);
	if (!f)
		return 1;

	char *content = get_file_content(f);
	printf("file content : \n---\n%s---\n", content);

	free_file(f, content);
	return 0;
}
