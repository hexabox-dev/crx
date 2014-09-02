// to compile, type:    gcc usage.c crx.o -o usage.exe

#include "crx.h"

int main()
{
     int   len;
     char  output[24];
     strncpy(output, regex("\\d*", "abc123xyz", &len), len);
     output[len] = 0x00;

     puts(output);
}
