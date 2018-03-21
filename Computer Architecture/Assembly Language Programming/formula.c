#include <stdio.h>
#include <stdlib.h>
#include "nCr.h"
#include <time.h>
#include <sys/time.h>

int main(int argc, const char * argv[])
{
	int val = 0;
	int n;
	int c;

	struct timeval start, end;

	if (argv[1][0] == '-') {
		printf("Usage: formula <positive integer>");
	}
	
	if (argv[1][1] == 'h') {
		printf("Usage: formula <positive integer>");

	} else {
		n = atoi(argv[1]);
		
		gettimeofday(&start, NULL);

		printf("(1 + x)^%i = ", n);

		if (n == 0)
			printf("0");

		for (; val <= n; val++) { //following does computation

			c = nCr(n, val);

			if (c == -1) {
				printf("Multiplication overflow. \n");
				return 1;
			} else {
				if (val != 0)
					printf("%i x^%i ",c , val);
				
				if (val != n && val != 0)
					printf("+ ");
			}
		}

		gettimeofday(&end, NULL);

	}

	printf("\n%ld microseconds\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));

	return 0;
}

