#include "akinator.hpp"

void DataGuess(Node** node, int* code_error) {

    if((*node)->left == NULL || (*node)->right == NULL) {
        EndOfGame(*node, code_error);
        return;
    }

    char answer[4] = {};

    fprintf(stderr, "%s?\n", (*node)->data);
    scanf("%s", answer);

    question:

        if(!strcmp(answer, "no")) {
            DataGuess(&(*node)->left, code_error);
        }
        else if(!strcmp(answer, "yes")) {
            DataGuess(&(*node)->right , code_error);
        }
        else {
            do {
                fprintf(stderr, "please, enter only 'yes' or 'no'\n");
                scanf("%s", answer);
            } while(strcmp(answer, "yes") && strcmp(answer, "no"));

            goto question;
        }
}

void EndOfGame(Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    fprintf(stderr, "is it %s?\n", node->data);

    char answer[4] = {};
    scanf("%s", answer);

    question:

        if(!strcmp(answer, "no")) {
            fprintf(stderr, "ne hype\n");
        }
        else if(!strcmp(answer, "yes")) {
            fprintf(stderr, "hype\n");
        }
        else {
            do {
                fprintf(stderr, "please, enter only 'yes' or 'no'\n");
                scanf("%s", answer);
            } while(strcmp(answer, "yes") && strcmp(answer, "no"));

            goto question;

        }
}