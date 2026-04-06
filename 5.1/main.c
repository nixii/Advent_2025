
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DA_INIT_CAP 4
#define DA_GROWTH_RATE 2
#define NREAD 4096

typedef struct longs {
	long *data;
	size_t len;
	size_t cap;
} longs;

longs longs_new() {
	return (longs) {
		.data = malloc(sizeof(long) * DA_INIT_CAP),
		.len = 0,
		.cap = DA_INIT_CAP
	};
}

void longs_resize(longs *l) {
	if (l->len < l->cap) return;
	if (l->cap == 0) return;
	l->data = realloc(l->data, sizeof(long) * l->cap * DA_GROWTH_RATE);
	l->cap *= DA_GROWTH_RATE;
}

void longs_append(longs *l, long v) {
	longs_resize(l);
	l->data[l->len++] = v;
}

void longs_destroy(longs *l) {
	free(l->data);
	l->data = NULL;
	l->cap = 0;
	l->len = 0;
}

int main(void)
{

	FILE *f = fopen("5.1/inp.txt", "r");
	if (f == NULL) {
		fprintf(stderr, "file not found\n");
		return 1;
	}

	longs ids = longs_new();
	longs mins = longs_new();
	longs maxs = longs_new();

	char buf[NREAD];
	while (fgets(buf, NREAD, f) != NULL) {
		
		char *end = NULL;
		long val = strtol(buf, &end, 0);
		if (end[0] == '-') {
			long endValue = strtol(end + 1, NULL, 0);
			longs_append(&mins, val);
			longs_append(&maxs, endValue);
		} else {
			longs_append(&ids, val);
		}
	}

	// print them out
	// for (int i = 0; i < mins.len; i++) {
	// 	printf("min %ld, max %ld\n", mins.data[i], maxs.data[i]);
	// }

	// check the ids
	int fresh = 0;
	for (int i = 0; i < ids.len; i++) {
		long id = ids.data[i];

		for (int j = 0; j < mins.len; j++) {
			if (id >= mins.data[j] && id <= maxs.data[j]) {
				fresh++;
				break;
			}
		}
	}

	printf("the number of fresh ingredients is %d of %zu\n", fresh, ids.len);

	longs_destroy(&ids);
	longs_destroy(&mins);
	longs_destroy(&maxs);

	return 0;
}