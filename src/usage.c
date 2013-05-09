// to compile, type:    gcc usage.c crx.o -o usage.exe

#include "crx.h"

int main()
{
     int   len;
     char  output[24];
     strncpy(output, regex("s\\d+", "sds12345", &len), len);
     output[len] = 0x00;

     puts(output);
}
