
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// dynamic array meanings
#define DA_MIN_SIZE 4
#define DA_GROWTH_RATE 2

// dynamic array of integers
typedef struct longs {
	long *data;
	size_t len;
	size_t cap;
} longs;

// create a new integer dynamic array
longs longs_new() {
	long *data = malloc(DA_MIN_SIZE * sizeof(long));
	return (longs){
		.len = 0,
		.cap = data == NULL ? 0 : DA_MIN_SIZE,
		.data = data
	};
}

// grow a dynarr
void longs_grow(longs *i) {
	if (i->cap == 0) return;
	if (i->len < i->cap) return;
	long *data = realloc(i->data, sizeof(long) * i->cap * DA_GROWTH_RATE);
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
void longs_append(longs *i, long in) {
	longs_grow(i);
	i->data[i->len] = in;
	i->len++;
}

// free the da
void longs_destroy(longs *i) {
	free(i->data);
	i->data = NULL;
	i->len = 0;
	i->cap = 0;
}

// 
int main(int argc, char **argv) {

	// store the values
	longs starts = longs_new();
	longs ends = longs_new();
	
	// open the file stream
	FILE *f = fopen("2.2/inp.txt", "r");
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
				longs *addedTo = c == '-' ? &starts : &ends;

				// finalize the string
				long value = strtol(numString, NULL, 10);
				memset(numString, 0, sizeof(numString));
				numStrIdx = 0;
				
				// add it
				longs_append(addedTo, value);
			}
		}
	}

	// double check the lengths
	assert(starts.len == ends.len);

	// a dynarr of all solutions
	longs solutions = longs_new();

	// loop for each range
	for (int i = 0; i < starts.len; i++) {
		
		// the starting and ending values
		long start = starts.data[i];
		long end = ends.data[i];

		// iterate through each number
		for (long i = start; i <= end; i++) {

			// check the integer to see if it follows the pattern.
			// if it follows the pattern then add it to the solutions.
			char longStr[17];
			memset(longStr, 0, sizeof(longStr));
			sprintf(longStr, "%li", i);

			// the length of the string
			size_t len = strlen(longStr);

			// iterate for each possibility
			for (int iter = 1; iter <= len / 2; iter++) {

				// do not continue if it is not a perfect multiple
				if (len % iter != 0) continue; 

				// the amount of times to duplicate the string
				int times = len / iter;

				// create the chars and clear it
				char dup[17];
				memset(dup, 0, sizeof(dup));

				// copy the memory
				for (size_t offset = 0; offset < times; offset++) memcpy(dup+offset*iter, longStr, iter);
				printf("copy of %s is %s --- <%d>\n", longStr, dup, iter);

				// check 
				if (strcmp(dup, longStr) == 0) {
					longs_append(&solutions, i);
					printf("ADDED: %li (%s)\n", i, longStr);
					break;
				}
			}
		}
		printf("\n\n");
	}

	// the final value to have
	long solution = 0;

	// sum the solution
	for (int i = 0; i < solutions.len; i++) {
		solution += solutions.data[i];
	}

	// print the solution
	printf("The solution is: %li\n", solution);

	// free the stuff
	longs_destroy(&solutions);
	longs_destroy(&starts);
	longs_destroy(&ends);

	// all done!
	return 0;
}