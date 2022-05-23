// to compile, type:    gcc unittest.c crx.o -o unittest.exe

#include "crx.h"

static const struct unittests {
    char *pattern;
    char *sample;
    int offset;
    int length;
} aTest[] = {
	{	"a*b",      	"aaaabx",       0,  5	},
	{	"a*b",      	"bx",           0,  1	},
	{	"a+b",      	"aaaabx",       0,  5	},
	{	"a?b",      	"abx",          0,  2	},
	{	"a?b",      	"bx",           0,  1	},
	{	"(ab)+c",   	"ababcd",       0,  5	},
	{	"(ab)+c",   	"xyzababc",     3,  5	},
	{	"a\\*c",    	"za*c",         1,  3	},
	{	"f..k",     	"zfolky",       1,  4	},
	{	"z.*b",     	"xyzababc",     2,  5	},
	{	"a?b",      	"aaab",         2,  2	},
	{	"a?a",      	"aa",           0,  2	},
	{	"a?",       	"a",            0,  1	},
	{	".*b",      	"bc",           0,  1	},
	{	"a*",       	"zb",           0,  0	},
	{	"[abc]",    	"zab",          1,  1	},
	{	"[^xyz]",   	"zab",          1,  1	},
	{	"[0-9]",    	"z8t",          1,  1	},
	{	"[0-9]+",   	"abc337xyz",    3,  3	},
	{	"[^a-z]",   	"123xyz",       0,  1	},

    {	"a{1,2}",   	"zb",          -1,  0	},
	{	"a{1,2}",   	"zab",          1,  1	},
	{	"a{1,2}",   	"zaab",         1,  2	},
	{	"a{1,2}",   	"zaaab",        1,  2	},
	{	"a{1}",     	"zb",          -1,  0	},
	{	"a{1}",     	"zab",          1,  1	},
	{	"a{1}",     	"zaab",         1,  1	},
	{	"a{1,}",    	"zb",          -1,  0	},
	{	"a{1,}",    	"zab",          1,  1	},
	{	"a{1,}",    	"zaab",         1,  2	},
	{	"a{,1}",    	"zb",           0,  0	},
	{	"a{,1}b",   	"zb",           1,  1	},
	{	"a{,1}b",   	"zab",          1,  2	},
	{	"a{,1}b",   	"zaab",         2,  2	},
	{	"a{2}",     	"zab",         -1,  0	},
	{	"a{2}",     	"zaab",         1,  2	},
	{	"a{2}",     	"zaaab",        1,  2	},

	{	"a\\d+b",   	"za789b",       1,  5	},
	{	"\\A+",     	"abc12..",      3,  4	},	// "12.."
	{	"\\A*",     	"abc12..",      0,  0	},	// ""
	{	"\\A*\\d",  	"abc12..",      3,  2	},	// "12"
	{	"\\A*.",    	"abc12..",      0,  1	},	// "a"
	{	"s*\\d+",   	"sd1234",       2,  4	},	// "1234"
	{	"s?\\d+",   	"sd1234",       2,  4	},	// "1234"
	{	"s+\\d+",   	"sd1234",      -1,  0	},
	{	"s\\d+",    	"sds1234",      2,  5	},
	{	"s\\d*",    	"sd1234",       0,  1	},
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
