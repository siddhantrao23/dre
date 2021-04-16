#ifndef __LIST_H_
#define __LIST_H_

struct reg_t {
    int type;
    union {
        char ch;
        char *ch_class;
    };
    struct reg_t *next;
};

struct list_t {
    struct reg_t *head;
};

struct reg_t* regnew(int type);
struct list_t* regcomp(char *pattern);
void print_l(struct list_t *l);
void free_l(struct list_t *l);

#endif // __LIST_H_
