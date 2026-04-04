#include <stdio.h>

// the amount of charges per bank
#define BANK_CHARGE_AMOUNT 100
#define BANK_AMOUNT 200

// finally run the code
int main(int argc, char **argv) {

	// open the file
	FILE *f = fopen("3.1/inp.txt", "r");

	// check it worked
	if (!f) {
		fprintf(stderr, "File failed to open.\n");
		return 1;
	}

	// allocate the bank storage
	int banks[BANK_AMOUNT * BANK_CHARGE_AMOUNT];

	// the best joltages stored
	int joltages[BANK_AMOUNT];

	// read buffer
	char readBuffer[4096];

	// the state of which bank is being read
	int currentBank = 0;
	int currentBattery = 0;

	// iterate through all the bytes
	size_t numRead;
	while ((numRead = fread(readBuffer, 1, sizeof(readBuffer), f)) != 0) {

		// iterate for all read bytes
		for (size_t readIdx = 0; readIdx < numRead; readIdx++) {

			// get the current character
			char c = readBuffer[readIdx];

			// newlines mean a new bank
			if (c == '\n') {
				currentBank++;
				currentBattery = 0;

			// otherwise it is just a new digit
			} else {
				int joltage = c - '0';
				banks[currentBank * BANK_CHARGE_AMOUNT + (currentBattery++)] = joltage;
			}
		}
	}

	// iterate through each bank to find the best joltage
	for (int bankIdx = 0; bankIdx < BANK_AMOUNT; bankIdx++) {

		// get the current bank
		int *bank = &banks[bankIdx * BANK_CHARGE_AMOUNT];

		// the current best solution
		int bestSolution = 0;

		// iterate for each battery in the bank, minus one (it isn't needed)
		// this is very slow (O(n^2) complexity) but it is the simplest solution
		for (int i = 0; i < BANK_CHARGE_AMOUNT - 1; i++) {
			
			// iterate again to find pairs
			for (int j = i + 1; j < BANK_CHARGE_AMOUNT; j++) {

				// calculate the value of this pair
				int currentValue = bank[i] * 10 + bank[j];

				// if it is smaller then set the best one
				if (currentValue > bestSolution) bestSolution = currentValue;
			}
		}

		// add the best solution to the joltages
		joltages[bankIdx] = bestSolution;
	}

	// sum up the joltages
	int joltageSum = 0;
	for (int joltageIndex = 0; joltageIndex < BANK_AMOUNT; joltageIndex++) {
		joltageSum += joltages[joltageIndex];
	}

	// print out the joltage total
	printf("\nTotal joltage: %d\n", joltageSum);

	// free everything used
	fclose(f);

	// all done!
	return 0;
}