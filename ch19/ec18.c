#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

/* Our old friend die from exercise 17 */
void die(const char *message)
{
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}
	
	exit(1);
}

// a typedef creates a fake type, in this case for a function pointer
typedef int (*compare_cb) (int a, int b);
typedef int *(*sort_cb) (int *numbers, int count, compare_cb cmp);

/* A classic bubble sort that uses the 
   compare_cb callback to do the sorting */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
	int temp = 0;
	int i = 0;
	int j = 0;
	int *target = malloc(count * sizeof(int));
	
	if (!target) die("Memory error.");
	
	memcpy(target, numbers, count * sizeof(int));
	
	for (i = 0; i < count; i++) {
		for (j = 0; j < count - 1; j++) {
			if (cmp(target[j], target[j+1]) > 0) {
				temp = target[j+1];
				target[j+1] = target[j];
				target[j] = temp;
			}
		}
	}
	return target;
}

// Small helper function
void swap(int *numbers, int index1, int index2)
{
	int temp = numbers[index2];
	numbers[index2] = numbers[index1];
	numbers[index1] = temp;
}

// Have to make a function to partition our list for
// recursive sorts.
int partition(int *numbers, int left, int right, int pivot, compare_cb cmp)
{
	int leftindex = left - 1;
	int rightindex = right;
	
	while (true) {
		
		while (cmp(numbers[++leftindex], pivot) < 0){
			// do nothing
		}
		
		while(rightindex > 0 && cmp(numbers[--rightindex], pivot) > 0) {
			// do nothing
		}
		
		if (leftindex >= rightindex) {
			break;
		} else {
			swap(numbers, leftindex, rightindex);
		}
		
	}
	
	swap(numbers, leftindex, right);
	return leftindex;
}

void quicksort_proc(int *numbers, int left, int right, compare_cb cmp)
{
	if (right - left < 0) {
		return;
	} else {
		int pivot = numbers[right];
		int breakPoint = partition(numbers, left, right, pivot, cmp);
		quicksort_proc(numbers, left, breakPoint - 1, cmp);
		quicksort_proc(numbers, breakPoint + 1, right, cmp);
	}
}

// For extra credit, I implemented quicksort.
int *quicksort(int *numbers, int count, compare_cb cmp)
{
	int *target = malloc(count * sizeof(int));
	memcpy(target, numbers, count * sizeof(int));
	
	quicksort_proc(target, 0, count-1, cmp);
	return target;
}

int sorted_order(int a, int b)
{
	return a - b;
}

int reverse_order(int a, int b)
{
	return b - a;
}

int strange_order(int a, int b)
{
	if (a == 0 || b == 0) {
		return 0;
	} else {
		return a % b;
	}
}

// Used to test that we are sorting things correctly by doing the sort and
// printing it out.

void test_sorting (int *numbers, int count, sort_cb sort, compare_cb cmp)
{
	int i = 0;
	int *sorted = sort(numbers, count, cmp);
	
	if (!sorted) die("Failed to sort as requested.");
	
	for (i = 0; i < count; i++) {
		printf("%d", sorted[i]);
	}
	printf("\n");
	
	free(sorted);
}

int main (int argc, char *argv[])
{
	if (argc < 2) die("USAGE: ./ex18 4 3 1 5 6");
	
	int count = argc - 1;
	int i = 0;
	char **inputs = argv + 1;
	
	int *numbers = malloc(count * sizeof(int));
	if (!numbers) die("Memory error.");
	
	for (i = 0; i < count; i++) {
		numbers[i] = atoi(inputs[i]);
	}
	
	test_sorting(numbers, count, bubble_sort, sorted_order);
	test_sorting(numbers, count, bubble_sort, reverse_order);
	test_sorting(numbers, count, bubble_sort, strange_order);
	
	test_sorting(numbers, count, quicksort, sorted_order);
	test_sorting(numbers, count, quicksort, reverse_order);
	test_sorting(numbers, count, quicksort, strange_order);
	
	free(numbers);
	return 0;
}
