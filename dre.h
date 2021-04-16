#ifndef __DRE_H_
#define __DRE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1001
#define MAX_CHAR_CLASS 10 // for a-zA-Z0-9

struct reg_t {
    int type;
    union {
        char ch;
        char *ch_class;
    };
    struct reg *next;
};

struct list_t {
    struct reg_t *head;
};

enum { DOT, CARET, DOLLAR, QUESTION, STAR, PLUS, CHAR, CHAR_CLASS, DIGIT, ALPHA, EOP };

// function declarations
int match_here(struct reg_t *, char *, int *);
int match_one(struct reg_t *, char);
int match_star(struct reg_t *, struct reg_t *, char *, int *);
int match_star_lazy(struct reg_t *, struct reg_t *, char *, int *);
int match_plus(struct reg_t *, struct reg_t *, char *, int *);
int match_question(struct reg_t *, struct reg_t *, char *, int *);
int match_char_class(char, char *);

struct reg_t *new_reg(int type);

#endif // __DRE_H_
