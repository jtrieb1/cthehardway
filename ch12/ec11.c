#include <stdio.h>

// Extra credit section

int main (int argc, char *argv[]) {
	
	// using a while loop this time
	
	int i = argc - 1;
	while (i > -1) {
		printf("arg %d: %s\n", i, argv[i]);
		i--;
	}

	// making our own array of strings
	char *states[] = {
		"California", "Oregon",
		"Washington", "Texas"
	};
	
	int num_states = 4;
	i = num_states - 1; // important to reset after prior loop
	
	while (i > -1) {
		printf("state %d: %s\n", i, states[i]);
		i--;
	}
	
	puts("Copying args into states array...");
	
	// time to copy our argv stuff into states
	i = 0; // reset i
	while (i < num_states) {
		states[i] = argv[i];
		i++;
	}
	
	// let's print them out to check if it worked.
	i = 0;
	while (i < num_states) {
		printf("state %d: %s\n", i, states[i]);
		i++;
	}


	return 0;
}
