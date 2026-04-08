
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// constants for the input file
#define NUM_PROBLEMS 1000
#define NUM_NUMBERS 4

// finally run
int main(void)
{

	// open the input
	FILE *f = fopen("6.1/inp.txt", "r");
	if (f == NULL) {
		fprintf(stderr, "couldn't open file.\n");
		return 1;
	}

	// allocate buffers
	long numbers[NUM_PROBLEMS * NUM_NUMBERS];
	int operations[NUM_PROBLEMS]; // 0 is add, 1 is mult
	int numCount = 0;
	int opCount = 0;

	// read buffers
	char line[8192];

	// read the lines
	while (fgets(line, sizeof(line), f) != NULL) {

		// while there is more to read
		char *next = line;
		while (*next != '\n' && *next != '\0') {

			// ignore if it is a space
			if (isspace(*next)) {
				next++;
			
			// read the number
			} else if (isdigit(*next)) {
				long l = strtol(next, &next, 0);
				numbers[numCount++] = l;
			
			// read the operations
			} else if (*next == '+') {
				operations[opCount++] = 0;
				next++;
			} else if (*next == '*') {
				operations[opCount++] = 1;
				next++;
			}
		}
	}

	// debug
	printf("%d numbers read with %d operations\n", numCount, opCount);

	// calculate everything
	long answers[NUM_PROBLEMS];
	for (int i = 0; i < NUM_PROBLEMS; i++) {

		// all the numbers to add
		long nums[NUM_NUMBERS] = {
			numbers[i], 
			numbers[i + NUM_PROBLEMS],
			numbers[i + NUM_PROBLEMS * 2],
			numbers[i + NUM_PROBLEMS * 3]
		};

		// do the right operation
		if (operations[i] == 0) {
			answers[i] = nums[0] + nums[1] + nums[2] + nums[3];
		} else {
			answers[i] = nums[0] * nums[1] * nums[2] * nums[3];
		}

		// print the anwer this time
		printf("\t\tThe answer is %ld\n", answers[i]);
	}

	// sum all the answers together
	long sum = 0;
	for (int i = 0; i < NUM_PROBLEMS; i++) {
		// printf("sum: %ld + %ld\n", sum, answers[i]);
		sum += answers[i];
	}

	// print the sum
	printf("The sum is %ld\n", sum);

	// done
	fclose(f);
	return 0;
}