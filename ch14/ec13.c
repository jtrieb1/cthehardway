#include <stdio.h>

// We're moving on to switch statements.

int main (int argc, char *argv[])
{
	if (argc != 2) {
		printf("ERROR: you need one argument.\n");
		return 1; // This is how you abort a program
	}
	
	int i = 0;
	char letter = ',';
	for (i=0; letter != '\0'; i++) {
		letter = argv[1][i] | (1<<5); // convert to lowercase
		
		switch(letter) {
			case 'a':
				printf("%d: A\n", i);
				break;
			case 'e':
				printf("%d: E\n", i);
				break;
			case 'i':
				printf("%d: I\n", i);
				break;
			case 'o':
				printf("%d: O\n", i);
				break;
			case 'u':
				printf("%d: U\n", i);
				break;
			case 'y':
				if (i > 2) { // it's only sometimes y
					printf("%d: Y\n", i);
					break;
				}
			default:
				printf("%d: %c is not a vowel.\n", i, letter);
		}
		letter = argv[1][i]; // reset so that we don't miss our null 						byte
	}

	return 0;
	
}
