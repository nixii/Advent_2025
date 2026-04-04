#include <stdio.h>
#include <math.h>
#include <string.h>

// the amount of charges per bank
#define BANK_CHARGE_AMOUNT 100
#define BANK_AMOUNT 200

// for part 2
#define ON_LEN 12

// turn a character into the digit it shows
#define INT(c) ((int)((c)-'0'))

// turn an integer into the character representing it; only digits
#define CHR(i) ((char)((i) + '0'))

// a minimum int
#define mini(a, b) ((a) > (b) ? (b) : (a))

// finally run the code
int main(int argc, char **argv) {

	// open the file
	FILE *f = fopen("3.2/inp-given.txt", "r");

	// check it worked
	if (!f) {
		fprintf(stderr, "File failed to open.\n");
		return 1;
	}

	// allocate the bank storage
	int banks[BANK_AMOUNT * BANK_CHARGE_AMOUNT];

	// the best joltages stored
	long joltages[BANK_AMOUNT];

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
				int joltage = INT(c);
				banks[currentBank * BANK_CHARGE_AMOUNT + (currentBattery++)] = joltage;
			}
		}
	}

	// read done
	printf("read done\n");

	// iterate through each bank to find the best joltage
	for (int bankIdx = 0; bankIdx < BANK_AMOUNT; bankIdx++) {

		// get the current bank
		int *bank = &banks[bankIdx * BANK_CHARGE_AMOUNT];

		// store the 12 largest numbers, initially just the first 12 numbers
		int largestNums[ON_LEN];
		memset(largestNums, 0, sizeof(largestNums));

		// iterate for all the numbers
		for (int batIndex = 0; batIndex < BANK_CHARGE_AMOUNT; batIndex++) {

			// the charge at this index
			int charge = bank[batIndex];

			// check where to put the number, only going as far as you can
			int targetIdx = -1;
			for (int i = 0; i < mini(ON_LEN, BANK_CHARGE_AMOUNT - batIndex); i++) {
				if (largestNums[i] < charge) targetIdx = i;
			}

			// set the index
			if (targetIdx != -1) {
				
				for (int i = 0; i < targetIdx; i++) {
					largestNums[i] = 0;
				}
				largestNums[targetIdx] = charge;
			}
		}

		// calculate the value (backwards)
		long val = 0;
		for (int i = 0; i < ON_LEN; i++) {
			val += powl(10, i) * largestNums[i];
		}

		// print it all out
		printf("val: %ld, original: ", val);
		for (int i = 0; i < BANK_CHARGE_AMOUNT; i++) {
			printf("%d", bank[i]);
		}
		printf("\n");

		// add the value in to the joltages
		joltages[bankIdx] = val;
	}

	// sum up the joltages
	long long joltageSum = 0;
	for (int joltageIndex = 0; joltageIndex < BANK_AMOUNT; joltageIndex++) {
		joltageSum += joltages[joltageIndex];
	}

	// print out the joltage total
	printf("\nTotal joltage: %lld\n", joltageSum);

	// free everything used
	fclose(f);

	// all done!
	return 0;
}