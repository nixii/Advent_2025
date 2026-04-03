
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// dynamic array meanings
#define DA_MIN_SIZE 4
#define DA_GROWTH_RATE 2

// dynamic array of integers
typedef struct ints {
	int *data;
	size_t len;
	size_t cap;
} ints;

// create a new integer dynamic array
ints ints_new() {
	int *data = malloc(DA_MIN_SIZE * sizeof(int));
	return (ints){
		.len = 0,
		.cap = data == NULL ? 0 : DA_MIN_SIZE,
		.data = data
	};
}

// grow a dynarr
void ints_grow(ints *i) {
	if (i->cap == 0) return;
	if (i->len < i->cap) return;
	int *data = realloc(i->data, sizeof(int) * i->cap * DA_GROWTH_RATE);
	if (data == NULL) {
		free(i->data);
		i->len = 0;
		i->cap = 0;
	} else {
		i->cap *= DA_GROWTH_RATE;
		i->data = data;
	}
}

// append to the da
void ints_append(ints *i, int in) {
	ints_grow(i);
	i->data[i->len] = in;
	i->len++;
}

// free the da
void ints_destroy(ints *i) {
	free(i->data);
	i->data = NULL;
	i->len = 0;
	i->cap = 0;
}

// 
int main(int argc, char **argv) {

	// store the values
	ints starts = ints_new();
	ints ends = ints_new();
	
	// open the file stream
	FILE *f = fopen("2/inp.txt", "r");
	if (f == NULL) return 1;
	
	// the buffer to store the read bytes
	char buf[4096];

	// store the string for the number
	char numString[33];
	int numStrIdx = 0;

	// clear the number string so it has null terminators
	memset(numString, 0, sizeof(numString));

	// the current amount of bytes read
	size_t numRead = 0;

	// loop while there are bytes to stream
	// this implementation relies on a trailing space else the final number is skipped
	while ((numRead = fread(buf, 1, sizeof(buf), f)) > 0) {

		// loop through the read bytes
		for (int i = 0; i < numRead; i++) {

			// get the current character
			char c = buf[i];

			// if c is a number then add it to the number string
			if (c >= '0' && c <= '9') {
				numString[numStrIdx++] = c;
			}

			// if it is not a number then it is ending a 
			else {

				// choose the correct array to add it to
				ints *addedTo = c == '-' ? &starts : &ends;

				// finalize the string
				int value = atoi(numString);
				memset(numString, 0, sizeof(numString));
				numStrIdx = 0;
				
				// add it
				ints_append(addedTo, value);
			}
		}
	}

	// double check the lengths
	assert(starts.len == ends.len);

	// a dynarr of all solutions
	ints solutions = ints_new();

	// loop for each range
	for (int i = 0; i < starts.len; i++) {
		
		// the starting and ending values
		int start = starts.data[i];
		int end = ends.data[i];

		// iterate through each number
		for (int i = start; i <= end; i++) {

			// TODO
			// check the integer to see if it follows the pattern.
			// if it follows the pattern then add it to the solutions.

		}
		printf("\n\n");
	}

	// the final value to have
	int solution = 0;

	// sum the solution
	for (int i = 0; i < solutions.len; i++) {
		solution += solutions.data[i];
	}

	// print the solution
	printf("The solution is: %d\n", solution);

	// all done!
	return 0;
}