#ifndef __UTIL_H_
#define __UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1001
#define MAX_CHAR_CLASS 10 // for a-zA-Z0-9

enum { DOT, CARET, DOLLAR, QUESTION, STAR, PLUS, CHAR, CHAR_CLASS, DIGIT, ALPHA, EOP };
static const char* types[] = {"DOT", "CARET", "DOLLAR", "QUESTION", "STAR", "PLUS", "CHAR", "CHAR_CLASS", "DIGIT", "ALPHA", "EOP" };

#endif // __UTIL_H_
