#include "dre.h"

struct reg_t *new_reg(int type) {
    struct reg_t *new = (struct reg_t *)malloc(sizeof(reg_t));
    new->type = type;
    new->next = NULL;

    return new;
}

struct list_t* pre_process(char *pattern) {
    struct list_t *list = (struct list_t *)malloc(sizeof(struct list_t));
    list->head = NULL;
    struct reg_t *curr = list->head;

    char c;
    char buffer[MAX_CHAR_CLASS];
    int buf_idx = 0;
    for (int i = 0; pattern[i] != '\0'; ++i) {
        c = pattern[i];
        struct reg_t *new;
        switch(c) {
            case '^': {
                new = new_reg(CARET);
            } break;
            case '$': {
                new = new_reg(DOLLAR);
            } break;
            case '.': {
                new = new_reg(DOT);
            } break;
            case '*': {
                new = new_reg(STAR);
            } break;
            case '+': {
                new = new_reg(PLUS);
            } break;
            case '?': {
                new = new_reg(QUESTION);
            } break;
            case '\\': {
                i++;
                switch(pattern[i]) {
                    case 'd': {
                        new = new_reg(DIGIT);
                    } break;
                    case 'w': {
                        new = new_reg(ALPHA);
                    } break;
                }
            } break;
            case '[': {
                while (pattern[++i] != ']') {
                    buffer[buf_idx++] = pattern[i];
                }
                buffer[buf_idx] = '\0';
                new = new_reg(CHAR_CLASS);
                new->ch_class = strdup(buffer);
                buf_idx = 0;
            } break;
            default: {
                new = new_reg(CHAR);
                new->ch = c;
            }
        }
        if(curr == NULL) {
            curr = new;
            list->head = new;
        } else {
            curr->next = new;
            curr = new;
        }
    }
    struct reg_t *last_node = new_reg(EOP);
    if(curr == NULL) {
        list->head = last_node;
    } else {
        curr->next = last_node;
    }

    return list;
}

void print_l(struct list_t *l) {
    struct reg_t *reg = l->head;
    const char* types[] = {"DOT", "CARET", "DOLLAR", "QUESTION", "STAR", "PLUS", "CHAR", "CHAR_CLASS", "DIGIT", "ALPHA", "EOP" };

    while (reg != NULL) {
        printf("type: %s", types[reg->type]);
        if(reg->type == CHAR_CLASS) {
            printf(" [%s]", reg->ch_class);
        } else if(reg->type == CHAR) {
            printf(" %c", reg->ch);
        }
        printf("\n");
        reg = reg->next;
    }
}

void free_l(struct list_t *l) {
    struct reg_t *reg = l->head;
    struct reg_t *del;
    while (reg != NULL) {
        if (reg->type == CHAR_CLASS) {
            free(reg->ch_class);
        }
        del = reg;
        reg = reg->next;
        free(del);
    }
    free(l);
}

void match(char *pattern, char *text) {
    struct list_t *l = pre_process(pattern);
    struct reg_t *curr = l->head;

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
    int m;
    int i = 0;
    while ((text[i] = getchar()) !='\n') {
        ++i;
    }
    text[i] = '\0';

    m = getchar() - '0';
    getchar();

    for (int t = 0; t<m; ++t) {
        i = 0;
        while ((pattern[i] = getchar()) !='\n') {
            ++i;
        }
        pattern[i] = '\0';

        struct list_t *l = pre_process(pattern);
        // print_l(l);
        match(pattern, text);
        free_l(l);
    }
}
