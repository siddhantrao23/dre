#ifndef __SPAN_H_
#define __SPAN_H_

#include <stdio.h>

struct span_t
{
    int valid;

    int start_idx;
    int end_idx;

    char *start;
    char *end;
};

struct span_t* spannew();

#endif // __SPAN_H_
