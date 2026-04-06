
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DA_INIT_CAP 4
#define DA_GROWTH_RATE 2
#define NREAD 4096

// make a dynamic array of any type macro
#define MAKE_DA(type, name) \
	typedef struct name { \
		type *data; \
		size_t len; \
		size_t cap; \
	} name; \
 \
	name name##_new() { \
		return (name) { \
			.data = malloc(sizeof(type) * DA_INIT_CAP), \
			.len = 0, \
			.cap = DA_INIT_CAP \
		}; \
	} \
 \
	void name##_resize(name *l) { \
		if (l->len < l->cap) return; \
		if (l->cap == 0) return; \
		l->data = realloc(l->data, sizeof(type) * l->cap * DA_GROWTH_RATE); \
		l->cap *= DA_GROWTH_RATE; \
	} \
 \
	void name##_append(name *l, type v) { \
		name##_resize(l); \
		l->data[l->len++] = v; \
	} \
 \
	void name##_destroy(name *l) { \
		free(l->data); \
		l->data = NULL; \
		l->cap = 0; \
		l->len = 0; \
	} \
 \
	void name##_remove(name *l, size_t idx) { \
		memmove( \
			l->data + idx, \
			l->data + idx + 1,  \
			(l->len - 1 - idx) * sizeof(type)); \
		l->len--; \
	}

// a single range, so only one dynamic array is used
typedef struct range {
	long min;
	long max;
} range;

// create the dynamic arrays
MAKE_DA(long, longs)
MAKE_DA(range, ranges)

// compare two ranges by their min value, then their max value if the min value
// is the same
int compare_range(const void *a, const void *b) {
	const range *rA = (const range *)a; 
	const range *rB = (const range *)b;
	if (rA->min < rB->min) return -1;
	if (rA->min > rB->min) return 1;
	if (rA->max < rB->max) return -1;
	if (rA->max > rB->max) return 1;
	return 0;
}

// combine an entire list of ranges together as much as possible
void combine_ranges(ranges *r) {

	// sort the entire list first
	qsort(r->data, r->len, sizeof(r->data[0]), compare_range);

	// this is a bit complicated so it will be a series of steps.
	//  1. get the first range and store it in cur.
	//  2. check cur and the one after it to see if they overlap
	//  3. if they overlap then combine them and remove the second one
	//  4. if cur does not overlap with the next one then move it over by one
	//  5. repeat 2-4 until cur is the final index in the list
	// this guarantees it will be combined fully
	range *cur = &r->data[0];
	while (cur != &r->data[r->len - 1]) {
		range *next = cur + 1;

		if (next->min <= cur->max) {
			if (next->max > cur->max)
				cur->max = next->max;
			ranges_remove(r, next - r->data);
		} else {
			cur++;
		}
	}
}

// finaly run the program
int main(void)
{

	// open the input file
	FILE *f = fopen("5.2/inp.txt", "r");
	if (f == NULL) {
		fprintf(stderr, "file not found\n");
		return 1;
	}

	// stores all the ranges in the file
	ranges allRanges = ranges_new();

	// read each line into the buffer (they shouldn't go over 4096 chars)
	char buf[NREAD];
	while (fgets(buf, NREAD, f) != NULL) {
		
		// init the end position
		char *end = NULL;

		// convert the start of the line into a long, and set the end position
		// to the character afterwards
		long val = strtol(buf, &end, 0);

		// if the end position is a '-' sign, it is a range
		if (end[0] == '-') {

			// get the maximum value of the range and append it
			long endValue = strtol(end + 1, NULL, 0);
			ranges_append(&allRanges, (range){.min = val, .max = endValue});
		}
	}

	// use a different list for the combined ranges
	ranges combinedRanges = ranges_new();

	// start it with one element
	ranges_append(&combinedRanges, allRanges.data[0]);

	// DEBUG: print out the entire list and its length
	printf("\n%zu \n", combinedRanges.len);
	for (int i = 0; i < combinedRanges.len; i++) {
		printf("%ld   %ld\n", combinedRanges.data[i].min, combinedRanges.data[i].max);
	}

	// add one element at a time and combine it in to limit the amount of
	// comparisons
	for (int i = 1; i < allRanges.len; i++) {
		ranges_append(&combinedRanges, allRanges.data[i]);
		combine_ranges(&combinedRanges);
	}

	// DEBUG: print out the entire list and its length
	printf("\n%zu \n", combinedRanges.len);
	for (int i = 0; i < combinedRanges.len; i++) {
		printf("%ld   %ld\n", combinedRanges.data[i].min, combinedRanges.data[i].max);
	}
	printf("\n\n");

	// the total amount of valid IDs
	long amnt = 0;

	// add the amount of IDs each range covers
	for (int i = 0; i < combinedRanges.len; i++) {
		amnt += combinedRanges.data[i].max - combinedRanges.data[i].min + 1;
	}

	// log it out!
	printf("Total amount is %ld\n", amnt);

	// destroy everything fto save memory (not important in this small program)
	ranges_destroy(&allRanges);
	ranges_destroy(&combinedRanges);
	return 0;
}