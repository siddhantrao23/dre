#include "util.h"
#include "list.h"

struct reg_t* regnew(int type) {
    struct reg_t *new = malloc(sizeof(struct reg_t));
    new->type = type;
    new->next = NULL;

    return new;
}

// traverse the pattern and compile it into usable format
struct list_t* regcomp(char *pattern) {
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
                new = regnew(CARET);
            } break;
            case '$': {
                new = regnew(DOLLAR);
            } break;
            case '.': {
                new = regnew(DOT);
            } break;
            case '*': {
                new = regnew(STAR);
            } break;
            case '+': {
                new = regnew(PLUS);
            } break;
            case '?': {
                new = regnew(QUESTION);
            } break;
            case '\\': {
                i++;
                switch(pattern[i]) {
                    case 'd': {
                        new = regnew(DIGIT);
                    } break;
                    case 'w': {
                        new = regnew(ALPHA);
                    } break;
                }
            } break;
            case '[': {
                while (pattern[++i] != ']') {
                    buffer[buf_idx++] = pattern[i];
                }
                buffer[buf_idx] = '\0';
                new = regnew(CHAR_CLASS);
                new->ch_class = strdup(buffer);
                buf_idx = 0;
            } break;
            default: {
                new = regnew(CHAR);
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
    struct reg_t *last_node = regnew(EOP);
    if(curr == NULL) {
        list->head = last_node;
    } else {
        curr->next = last_node;
    }
    return list;
}

// print the regex list structure (for debugging)
void print_l(struct list_t *l) {
    struct reg_t *reg = l->head;
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
