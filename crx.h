/*
 * name: crx
 * description: Regular Expression Engine (light weight version) for C Language, using double-recursion and function pointers.
 * author: ken (hexabox) seto
 * date: 2009.08~09
 * license: BSD, GPL
 * version: 0.13.11
 */

#include <stdio.h>

#ifndef __CRX_H
#define __CRX_H

typedef enum {false, true} bool;

bool regex(char* pat, char* sam, char** loc, int* len);

#endif
