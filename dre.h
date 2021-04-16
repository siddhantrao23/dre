#ifndef __DRE_H_
#define __DRE_H_

#include "list.h"

//    function declarations
int match_here(struct reg_t *, char *, int *);
int match_one(struct reg_t *, char);
int match_star(struct reg_t *, struct reg_t *, char *, int *);
int match_star_lazy(struct reg_t *, struct reg_t *, char *, int *);
int match_plus(struct reg_t *, struct reg_t *, char *, int *);
int match_question(struct reg_t *, struct reg_t *, char *, int *);
int match_char_class(char, char *);


#endif // __DRE_H_
