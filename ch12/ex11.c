#include <stdio.h>

int main (int argc, char *argv[]) {
	
	// using a while loop this time
	
	int i = 0;
	while (i < argc) {
		printf("arg %d: %s\n", i, argv[i]);
		i++;
	}

	// making our own array of strings
	char *states[] = {
		"California", "Oregon",
		"Washington", "Texas"
	};
	
	int num_states = 4;
	i = 0; // important to reset to zero after prior loop
	
	while (i < num_states) {
		printf("state %d: %s\n", i, states[i]);
		i++;
	}
	
	return 0;
}
