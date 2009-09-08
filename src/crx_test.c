/*
 * name: crx_test
 * description: tester for crx, the light weight version of Regular Expression for C language.
 * author: ken (hexabox) seto
 * date: 2009.08~09
 */

#include "crx.h"

int main(int argc, char* argv[])
{
    char* loc;
    int len;
    bool success;

    printf("cRX tester\n");
    printf("==========\n");

    if (argc < 3)
    {
        printf("syntax:\ncrx_test <pattern> <string>\n");
        return 1;
    }

    success = regex(argv[1], argv[2], &loc, &len);

    printf("search '%s' in '%s'\n", argv[1], argv[2]);
    printf("> %s\n", success ? "SUCCESS" : "FAILED");

    if (success)
    {
        printf("found string : %s\n", loc);
        printf("match offset : %d\n", loc - argv[2]);
        printf("match length : %d\n", len);
    }
    return 0;
}
