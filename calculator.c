#include "stdlib.h"
#include "reader.h"

int calculate_result(struct Reader *reader) {
    struct Token *head = NULL;
    // create a linkedlist based stack of tokens.
    // pop when a operator is encountered and add the result to the stack
    while (reader->token != NULL) {
        reader->token->next = head;
        head = reader->token;
        if (head->tok_type != TOK_NUM) {
            // this is an operation:
            struct Token *op = head;
            struct Token *val_a = head->next;
            struct Token *val_b = head->next->next;

            struct Token *new_token = malloc(sizeof(struct Token));
            if (!new_token) {
                reader->had_error = true;
                return -1;
            }
            switch (op->tok_type) {
                case TOK_ADD:
                    new_token->val = val_a->val + val_b->val;
                    break;
                case TOK_DIV:
                    if (val_a->val != 0)
                        new_token->val = val_b->val / val_a->val;
                    else {
                        reader->had_error = true;
                        return -1;
                    }
                    break;
                case TOK_MULT:
                    new_token->val = val_a->val * val_b->val;
                    break;
                case TOK_SUB:
                    new_token->val = val_b->val - val_a->val;
                    break;
                default:
                    return -1;
            }
            new_token->next = val_b->next;
            new_token->tok_type = TOK_NUM;
            free(val_a);
            free(val_b);
            free(op);
            head = new_token;
        }
        advance(reader);
    }
    if (head != NULL && !head->next) {
        return head->val;
    } else {
        reader->had_error = true;
        return -1;
    }
}