// to compile, type:    gcc usage.c crx.o -o usage.exe

#include "crx.h"
#include <string.h>

int main()
{
     int   len;
     char  output[24];
	 const char* found = regex("s\\d+", "sds12345", &len);
     strncpy(output, found, len);
     output[len] = 0x00;

     puts(output);
}
