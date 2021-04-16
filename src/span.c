#include "span.h"

struct span_t* spannew(char *start) {
    struct span_t *span = malloc(sizeof(struct span_t));
    span->valid = 0;
    span->start = span->end = start;
    span->start_idx = span->end_idx = -1;
    return span;
}
