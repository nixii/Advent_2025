
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DA_INIT_CAP 4
#define DA_GROWTH_RATE 2
#define NREAD 4096

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

typedef struct range {
	long min;
	long max;
} range;

MAKE_DA(long, longs)
MAKE_DA(range, ranges)

int compare_range(const void *a, const void *b) {
	const range *rA = (const range *)a; 
	const range *rB = (const range *)b;
	if (rA->min < rB->min) return -1;
	if (rA->min > rB->min) return 1;
	if (rA->max < rB->max) return -1;
	if (rA->max > rB->max) return 1;
	return 0;
}

void combine_ranges(ranges *r) {
	qsort(r->data, r->len, sizeof(r->data[0]), compare_range);

	// 10 15
	// 20 17
	// ^  /
	// 

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

int main(void)
{

	FILE *f = fopen("5.2/inp.txt", "r");
	if (f == NULL) {
		fprintf(stderr, "file not found\n");
		return 1;
	}

	longs ids = longs_new();
	ranges allRanges = ranges_new();

	char buf[NREAD];
	while (fgets(buf, NREAD, f) != NULL) {
		
		char *end = NULL;
		long val = strtol(buf, &end, 0);
		if (end[0] == '-') {
			long endValue = strtol(end + 1, NULL, 0);
			ranges_append(&allRanges, (range){.min = val, .max = endValue});
		} else {
			longs_append(&ids, val);
		}
	}

	ranges combinedRanges = ranges_new();
	ranges_append(&combinedRanges, allRanges.data[0]);

	printf("\n%zu \n", combinedRanges.len);
	for (int i = 0; i < combinedRanges.len; i++) {
		printf("%ld   %ld\n", combinedRanges.data[i].min, combinedRanges.data[i].max);
	}

	for (int i = 1; i < allRanges.len; i++) {
		ranges_append(&combinedRanges, allRanges.data[i]);
		combine_ranges(&combinedRanges);
	}

	printf("\n%zu \n", combinedRanges.len);
	for (int i = 0; i < combinedRanges.len; i++) {
		printf("%ld   %ld\n", combinedRanges.data[i].min, combinedRanges.data[i].max);
	}

	printf("\n\n");

	long amnt = 0;

	for (int i = 0; i < combinedRanges.len; i++) {
		amnt += combinedRanges.data[i].max - combinedRanges.data[i].min + 1;
	}

	printf("Total amount is %ld\n", amnt);

	longs_destroy(&ids);
	ranges_destroy(&allRanges);
	ranges_destroy(&combinedRanges);

	return 0;
}