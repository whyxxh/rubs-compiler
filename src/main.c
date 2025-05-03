#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileIO.h"

int main(int argc, char **argv) 
{ 
	FILE *f = open_file(argc, argv);
	if (!f)
		return 1;

	char *content = get_file_content(f);
	if (content)
		printf("file content : \n---\n%s---\n", content);
	else
		return 1;

	free_file(f, content);
	return 0;
}
