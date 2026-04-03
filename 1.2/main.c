
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int  div_floor(int x, int y)
{
	return (int)floor((float)x / (float)y);
}

int main(void)
{

	// open the file
	FILE *f = fopen("1_Secret_Entrance/inp.txt", "r");
	
	// check
	if (f == NULL)
	{
		fprintf(stderr, "Failed to open the file.\n");
		goto done;
	}
	
	// create the dial, starting at 50.
	int dial = 50;
	
	// code
	int code = 0;

	// the buffer for each line
	char buffer[8];
	memset(buffer, 0, sizeof(buffer));
	
	// how to turn
	int dir;
	int dist;
	char strDist[8];
	memset(strDist, 0, sizeof(strDist));
	
	// iterate through the file
	size_t numRead;
	int lastStart = 0;
	while ((numRead = fread(buffer, 1, sizeof(buffer), f)) > 0)
	{
		
		// for all read bytes
		for (int i = 0; i < numRead; i++)
		{
		
			// get the character
			char c = buffer[i];
			
			// end the line and go back
			switch(c)
			{
			case '\n':
			
				// turn
				strDist[i-1] = 0; // add a new null terminator
				dist = atoi(strDist);
				
				// perform the turn
				int prev = dial;
				int next = dial + dir * dist;
				
				int wraps = abs(div_floor(next, 100) - div_floor(prev, 100));
				code += wraps;
				
				dial = ((next % 100) + 100) % 100;
				
				// reset
				memset(strDist, 0, sizeof(strDist));
				dir = 0;
			
				// update the buffer
				lastStart += i + 1;
				fseek(f, lastStart, SEEK_SET);
				printf("\tNEXT\n");
				goto nextIter;
			case 'L':
				dir = -1;
				break;
			case 'R':
				dir = 1;
				break;
			case '0'...'9':
				strDist[i-1] = c;
				break;
			}
		}
		
		nextIter:
		continue;
	}
	
	// print the code
	printf("The code is: %d\n", code);

// cleanup	
done:
	fclose(f);
	return 0;
}