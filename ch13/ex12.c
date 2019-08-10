#include <stdio.h>

int main (int argc, char *argv[]) 
{
	
	if (argc == 2) {
		printf("You only have one argument.\n");
	} else if (argc > 2 && argc < 5) {
		printf("Here's your arguments:\n");
		for (int i = 1; i < argc; i++) {
			printf("%s ", argv[i]);
		}
		printf("\n");
	} else {
		printf("You have too many arguments.\n");	
	}
	
	return 0;
}
