// to compile, type:    gcc unittest.c crx.o -o unittest.exe

#include "crx.h"

static const struct unittests {
    char *pattern;
    char *sample;
    int offset;
    int length;
} aTest[] = {
	{(char*)"a*b",      (char*)"aaaabx",       0, 5},
	{(char*)"a*b",      (char*)"bx",           0, 1},
	{(char*)"a+b",      (char*)"aaaabx",       0, 5},
	{(char*)"a?b",      (char*)"abx",          0, 2},
	{(char*)"a?b",      (char*)"bx",           0, 1},
	{(char*)"(ab)+c",   (char*)"ababcd",       0, 5},
	{(char*)"(ab)+c",   (char*)"xyzababc",     3, 5},
	{(char*)"a\\*c",    (char*)"za*c",         1, 3},
	{(char*)"f..k",     (char*)"zfolky",       1, 4},
	{(char*)"z.*b",     (char*)"xyzababc",     2, 5},
	{(char*)"a?b",      (char*)"aaab",         2, 2},
	{(char*)"a?a",      (char*)"aa",           0, 2},
	{(char*)"a?",       (char*)"a",            0, 1},
	{(char*)".*b",      (char*)"bc",           0, 1},
	{(char*)"a*",       (char*)"zb",           0, 0},
	{(char*)"[abc]",    (char*)"zab",          1, 1},
	{(char*)"[^xyz]",   (char*)"zab",          1, 1},
	{(char*)"[0-9]",    (char*)"z8t",          1, 1},
	{(char*)"[0-9]+",   (char*)"abc337xyz",    3, 3},

    {(char*)"a{1,2}",   (char*)"zb",          -1, 0},
	{(char*)"a{1,2}",   (char*)"zab",          1, 1},
	{(char*)"a{1,2}",   (char*)"zaab",         1, 2},
	{(char*)"a{1,2}",   (char*)"zaaab",        1, 2},
	{(char*)"a{1}",     (char*)"zb",          -1, 0},
	{(char*)"a{1}",     (char*)"zab",          1, 1},
	{(char*)"a{1}",     (char*)"zaab",         1, 1},
	{(char*)"a{1,}",    (char*)"zb",          -1, 0},
	{(char*)"a{1,}",    (char*)"zab",          1, 1},
	{(char*)"a{1,}",    (char*)"zaab",         1, 2},
	{(char*)"a{,1}",    (char*)"zb",           0, 0},	// DO match
	{(char*)"a{,1}b",   (char*)"zb",           1, 1},
	{(char*)"a{,1}b",   (char*)"zab",          1, 2},
	{(char*)"a{,1}b",   (char*)"zaab",         2, 2},
	{(char*)"a{2}",     (char*)"zab",         -1, 0},
	{(char*)"a{2}",     (char*)"zaab",         1, 2},
	{(char*)"a{2}",     (char*)"zaaab",        1, 2},

	{(char*)"[^a-z]",   (char*)"123xyz",       0, 1},
	{(char*)"a\\d+b",   (char*)"za789b",       1, 5},
	{(char*)"\\A+",     (char*)"abc12..",      3, 4},	// "12.."
	{(char*)"\\A*",     (char*)"abc12..",      0, 0},	// ""
	{(char*)"\\A*\\d",  (char*)"abc12..",      3, 2},	// "12"
	{(char*)"\\A*.",    (char*)"abc12..",      0, 1},	// "a"
	{(char*)"s*\\d+",   (char*)"sd1234",	   2, 4},	// "1234"
	{(char*)"s?\\d+",   (char*)"sd1234",	   2, 4},	// "1234"
	{(char*)"s+\\d+",   (char*)"sd1234",	  -1, 0},
	{(char*)"s\\d+",    (char*)"sds1234",	   2, 5},
	{(char*)"s\\d*",    (char*)"sd1234",	   0, 1},
};


int main()
{
    int   len;
    int   offset=-1;
    char  *matched;
    char  output[100];

	int numcases = (int)sizeof(aTest)/sizeof(aTest[0]);
	printf("number of cases: %d\n", numcases);

	int count_failed = 0;

    for( int i = 0 ; i < numcases ;  ++i )
	{
        matched = regex(aTest[i].pattern, aTest[i].sample, &len);
		// successful if return a pointer  i.e. not NULL

        offset = matched ? matched - aTest[i].sample : EOF;

        if (offset == aTest[i].offset && len == aTest[i].length) {

#ifdef DEBUG
			// no printout if passed to avoid verbose
            printf("Test [%s][%s] passed.\n", aTest[i].sample, aTest[i].pattern);
#endif
        } else {
			count_failed++;

			printf("case %3d \t", i+1);
            printf("in string [%s], search for [%s] FAILED.  Expected %d(%d), got %d(%d)\n",
						aTest[i].sample,
						aTest[i].pattern,
						aTest[i].offset,
						aTest[i].length,
						offset,
						len
					);
        }
    }

	if (count_failed == 0)
		puts("ALL PASSED");
	else
		printf("Failed %d case(s)\n", count_failed);
}
