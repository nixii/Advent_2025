
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COLS 136
#define ROWS 136
#define NOT_ROLL 9

#define PRINT_NUM_ARRAY \
	for (int x = 0; x < ROWS; x++) { \
		for (int y = 0; y < COLS; y++) { \
			if (neighborCount[x * COLS + y] == 9) printf("."); \
			else printf("%d", neighborCount[x * COLS + y]); \
		} \
		printf("\n"); \
	}

void paperRollAt(int *neighborCount, int row, int col, int amount) {
	for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
		if (row + rowOffset < 0 || row + rowOffset >= ROWS) continue;
		for (int colOffset = -1; colOffset <= 1; colOffset++) {
			if (rowOffset == 0 && colOffset == 0) continue;
			if (col + colOffset < 0 || col + colOffset >= COLS) continue;
			if (neighborCount[(row + rowOffset) * COLS + col + colOffset] == NOT_ROLL) continue;
			neighborCount[(row + rowOffset) * COLS + col + colOffset] += amount;
		}
	}
}

void addPaperRollAt(int *neighborCount, int row, int col) {
	paperRollAt(neighborCount, row, col, 1);
}
void removePaperRollAt(int *neighborCount, int row, int col) {
	paperRollAt(neighborCount, row, col, -1);
}

int main(void)
{

	// open the file
	FILE *f = fopen("4.2/inp.txt", "r");
	if (f == NULL) {
		fprintf(stderr, "file not found\n");
		return 1;
	}

	// allocate memory
	int *neighborCount = malloc(COLS * ROWS * sizeof(int));
	memset(neighborCount, 0, ROWS * COLS * sizeof(int));
	char buf[COLS + 1];

	// the total removable
	int numCanRemove = 0;
	int removedAny = 0;

	// rows and cols
	int row = 0, col = 0;

	// read the bytes
	size_t nRead;
	while ((nRead = fread(buf, 1, sizeof(buf), f)) != 0) {
		
		int newRowTimeYayyy = 0;

		// iterate over the read bytes
		for (size_t i = 0; i < nRead; i++) {
			char c = buf[i];
			
			switch(c) {
			case '\n':
				newRowTimeYayyy = 1;
				break;
			case '.':
				neighborCount[row * COLS + col] = NOT_ROLL;
				break;
			case '@':
				addPaperRollAt(neighborCount, row, col);
				break;
			}

			if (newRowTimeYayyy) {
				newRowTimeYayyy = 0;
				col = 0;
				row++;
				continue;
			}
			col++;
		}
	}

	// close the file
	fclose(f);

	// calc the number of removable rolls
	PRINT_NUM_ARRAY

	// get the amount of valid rolls
	int valid = 0;
	for (int i = 0; i < ROWS * COLS; i++) {
		if (neighborCount[i] <= 3) valid++;
	}

	// print the output
	printf("The amount of valid rolls is %d\n", valid);

	// iterate 
	do {
		removedAny = 0;

		for (int rowNum = 0; rowNum < ROWS; rowNum++) {
			for (int colNum = 0; colNum < COLS; colNum++) {
				if (neighborCount[rowNum * COLS + colNum] <= 3) {
					removePaperRollAt(neighborCount, rowNum, colNum);
					neighborCount[rowNum * COLS + colNum] = NOT_ROLL;
					removedAny = 1;
					numCanRemove++;
				}
			}
		}
	} while (removedAny);

	PRINT_NUM_ARRAY

	// the amount removable
	printf("The amount that can be removed in total is %d\n", numCanRemove);

	return 0;
}