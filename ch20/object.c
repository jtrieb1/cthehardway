#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "object.h"

void Object_destroy(void *self)
{
	Object *obj = self;
	if (obj) {
		if (obj->description) free(obj->description);
		free(obj);
	}
}

void Object_describe(void *self)
{
	Object *obj = self;
	printf("%s.\n", obj->description);
}

int Object_init(void *self)
{
	// do nothing really
	return 1;
}

void *Object_move(void *self, Direction direction)
{
	printf("You can't go that direction.\n");
	return NULL;
}

int Object_attack(void *self, int damage)
{
	printf("You can't attack that.\n");
	return 0;
}

void *Object_new(size_t size, Object proto, char *description)
{
	// need a description
	assert(description != NULL);
	
	// setup the default functions in case they haven't been set
	if (!proto.init) proto.init = Object_init;
	if (!proto.describe) proto.describe = Object_describe;
	if (!proto.destroy) proto.destroy = Object_destroy;
	if (!proto.move) proto.move = Object_move;
	if (!proto.attack) proto.attack = Object_attack;
	
	// This seems weird, but we can make a struct of one size,
	// then point a different pointer at it to "cast" it
	Object *el = calloc(1, size);
	assert(el != NULL);
	*el = proto;
	
	// copy the description over
	el->description = strdup(description);
	
	// initialize it with whatever init we were given
	if (!el->init(el)){
		// looks like it didn't initialize
		el->destroy(el);
		return NULL;
	} else {
		// we've made an object of any type
		assert(el != NULL);
		return el;
	}
}
