
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	ints start = ints_new();
	ints ends = ints_new();
	
	// open the file stream
	FILE *f = fopen("2/inp.txt", "r");
	if (f == NULL) return 1;
	
	// read the numbers
	char buf[4096];
	char numStr[4096];
	int numStrI = 0;
	memset(numStr, 0, sizeof(numStr));
	size_t nread;
	while ((nread = fread(buf, 1, sizeof(buf), f)) > 0) {
		for (int i = 0; i < nread; i++) {
			char c = buf[i];
			switch (c)
			{
			case '0'...'1':
				numStr[numStrI++] = c;
				break;
			default:
				printf("str: %s\n", numStr);
				memset(numStr, 0, sizeof(numStr));
				numStrI = 0;
				break;
			}
		}
	}

	return 0;
}