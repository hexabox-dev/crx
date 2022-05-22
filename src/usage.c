/*
 * to compile:
 *	   gcc usage.c crx.o -o usage.exe
 */

#include <string.h>
#include <string.h>
#include <assert.h>

#include "crx.h"

#define BUFLEN 24

int main()
{
     int  len;
     char output[BUFLEN];

	 char sample[] = "sds12345";
	 char pattern[] = "s\\d+";

	 const char* found = regex(pattern, sample, &len);

	 if (found) {
		 assert(len < BUFLEN);

		 long offset = found - sample;
		 strncpy(output, found, len);
		 output[len] = '\0';

		 printf("found at offset %ld >%s<\n", offset, output);

	 } else {
		 puts("not found");
	 }
}
