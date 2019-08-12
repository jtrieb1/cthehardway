#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Converting to instantiate the struct on the stack instead

struct Person {
	char *name;
	int age;
	int height;
	int weight;
};

// We no longer need malloc(), so these methods are commented out

/* struct Person *Person_create (char *name, int age, int height, int weight)
{
	// Allocate the proper amount of memory for the struct
	struct Person *who = malloc(sizeof(struct Person));
	// This should succeed, so who should not be null
	assert(who != NULL);
	// Assign each field of the struct
	who->name = strdup(name);
	who->age = age;
	who->height = height;
	who->weight = weight;
	
	return who;
}

void Person_destroy (struct Person *who)
{
	// If it's null, we don't have to free it
	assert(who != NULL);
	// who->name is also on the heap and has to be freed separately
	free(who->name);
	free(who);
}
*/

// Converted to not use pointers
void Person_print (struct Person who)
{
	printf("Name: %s\n", who.name);
	printf("\tAge: %d\n", who.age);
	printf("\tHeight: %d\n", who.height);
	printf("\tWeight: %d\n", who.weight);
}

int main (int argc, char *argv[])
{
	// Declare two Person structs
	struct Person joe;
	joe.name = "Joe Alex";
	joe.age = 32;
	joe.height = 64;
	joe.weight = 140;
	struct Person frank;
	frank.name = "Frank Blank";
	frank.age = 20;
	frank.height = 72;
	frank.weight = 180;
	
	// Print them out with information
	Person_print(joe);
	
	Person_print(frank);
	
	// make everyone age 20 years and print again
	
	joe.age    += 20;
	joe.height -= 2;
	joe.weight += 40;
	Person_print(joe);
	
	frank.age    += 20;
	frank.weight += 20;
	Person_print(frank);
	
	return 0;

}
