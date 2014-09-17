/*
compile:
    gcc crx.c test_crx.c -o test_crx
or 
    g++ crx.c test_crx.c -o test_crx+
*/
#include <stdio.h>
#include <string.h>
#include "crx.h"

int main() 
{
    int  len;
    char output[24];
    char pattern[] = "\\d*";
    char sample[] = "abc123";

    char* found = regex(pattern, sample, &len);
    if (found) {
        strncpy(output, found, len);
        output[len] = 0x00;
        printf("found >%s<\n", found);
    }
    else {
        printf("not found");
    }
    return 0;
}
