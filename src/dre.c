#include "dre.h"
#include "util.h"
#include "span.h"

void match(char *pattern, char *text) {
    struct list_t *l = regcomp(pattern);
    struct reg_t *curr = l->head;

    struct span_t *span = spannew(text);
    int found = 0;
    int beg = -1;
    int end = -1;
    if (curr->type == CARET) {
        beg += 1;
        found = match_here(curr->next, text, &end);
    } else {
        do {
            beg += 1;
            found = match_here(curr, text, &end);
            if (found == 1) {
                break;
            }
        } while (*text++ != '\0');
    }

    if (found == 0) {
        printf("%d\n", found);
    } else {
        if (end < 0)
            printf("%d %d %d\n", found, beg, beg + end + 1);
        else
            printf("%d %d %d\n", found, beg, beg + end);
    }
    free_l(l);
}

int match_here(struct reg_t *curr, char *text, int *end) {
    struct reg_t *prev = NULL;
    struct reg_t *next = NULL;
    int pre = *end;
    do {
        prev = curr;
        if (curr->next != NULL) {
            curr = curr->next;
            if (curr->next != NULL) {
                next = curr->next;
            } else {
                next = NULL;
            }
        } else {
            curr = NULL;
        }

        if (prev->type == EOP) {
            return match_question(prev, next, text, end);
        } else if (next != NULL && curr->type == STAR) {
            if (next->type == QUESTION)
                return match_star_lazy(prev, next->next, text, end);
            else
                return match_star(prev, next, text, end);
        } else if (next != NULL && curr->type == PLUS) {
            return match_plus(prev, next, text, end);
        } else if (prev->type == DOLLAR && curr->type == EOP) {
            return text[0] ==  '\0';
        }
        (*end)++;
    } while (text[0] != '\0' && match_one(prev, *text++) && curr != NULL);

    *end = pre;
    return 0;
}

int match_one(struct reg_t *curr, char c) {
    switch (curr->type) {
        case DOT: return 1;
        case CHAR_CLASS: return match_char_class(c, curr->ch_class);
        case DIGIT: return (c >= '0' && c <= '9');
        case ALPHA: return  ((c >= 'a' && c <= 'z') ||
                             (c >= 'A' && c <= 'Z') ||
                             (c >= '0' && c <= '9'));
        default: return curr->ch == c;
    }
}

int match_star(struct reg_t *curr, struct reg_t *n_next, char *text, int *end) {
    char *t;
    int pre = *end;
    for (t = text; *t != '\0' && match_one(curr, *t); ++t) {
        (*end)++;
    }
    do {
        // return rightmost match
        if (match_here(n_next, t, end)) {
            return 1;
        }
        (*end)--;
    } while (t-- > text);
    *end = pre;
    return 0;
}

int match_star_lazy(struct reg_t *curr, struct reg_t *n_next, char *text, int *end) {
    int pre = *end;
    do {
        if (match_here(n_next, text, end))
            return 1;
    } while (*text != '\0' && match_one(curr, *text++));
    *end = pre;
    return 0;
}

int match_plus(struct reg_t *curr, struct reg_t *n_next, char *text, int *end) {
    char *t;
    for (t = text; *t != '\0' && match_one(curr, *t); ++t) {
        (*end)++;
    }
    while (t > text) {
        if (match_here(n_next, t--, end)) {
            return 1;
        }
        (*end)--;
    }
    return 0;
}

int match_question(struct reg_t *curr, struct reg_t *n_next, char *text, int *end) {
    if (curr->type == EOP || match_here(n_next, text, end)) {
        return 1;
    }
    if (*text != '\0' && match_one(curr, *text++)) {
        if (match_here(n_next, text, end)) {
            (*end)++;
            return 1;
        }
    }
    return 0;
}

int match_char_class(char c, char *class) {
    do {
        if (c != '-' && class[0] != '\0' && class[0] != '-'
            && class[1] == '-' && class[2] != '\0'
            && c >= class[0] && c <= class[2]) {
            return 1;
        } else if (c == class[0]) {
            return 1;
        }
    } while (*class++ != '\0');
    return 0;
}

int main() {
    char text[4001];
    char pattern[1001];

    int i = 0;
    while ((text[i] = getchar()) !='\n') {
        ++i;
    }
    text[i] = '\0';

    i = 0;
    while ((pattern[i] = getchar()) !='\n') {
        ++i;
    }
    pattern[i] = '\0';

    // struct list_t *l = regcomp(pattern);
    // print_l(l);
    match(pattern, text);
    // free_l(l);
}
