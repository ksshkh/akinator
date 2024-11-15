#include "akinator.hpp"
#include "utils.hpp"

void AkinatorRun(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    fprintf(stderr, "hi, let's play the game\n");

    while(true) {
        fprintf(stderr, "press\nq: for quit g: for guess character d: for get definition c: for compare two characters\n");
        CleanBuffer();

        char answer = 0;

        while(scanf("%c", &answer)) {

            while(answer != 'q' && answer != 'g' && answer != 'd' && answer != 'c') {
                CleanBuffer();
                fprintf(stderr, "pls, use only letters q, g, d, c\n");
                scanf("%c", &answer);
            }

            if(answer == 'q') {
                fprintf(stderr, "bye bye\n");
                return;
            }
            else if(answer == 'g') {
                DataGuess(tree, &(tree->root), code_error);
                break;
            }
            else if(answer == 'd') {
                fprintf(stderr, "enter name of a character\n");

                char character = 0;
                scanf("%s", &character);

                GetDefinition(tree, &character, code_error);
                break;
            }
            else if(answer == 'c') {
                fprintf(stderr, "enter name of the first character\n");

                char character1 = 0;
                scanf("%s", &character1);
                fprintf(stderr, "!%s!\n", &character1);

                fprintf(stderr, "enter name of the second character\n");

                char character2 = 0;
                scanf("%s", &character2);
                fprintf(stderr, "!%s!\n", &character2);

                WordsCompare(tree, &character1, &character2, code_error);
                break;
            }
        }
    }

}

void DataGuess(Tree* tree, Node** node, int* code_error) {

    if((*node)->left == NULL || (*node)->right == NULL) {
        EndOfGame(tree, *node, code_error);
        return;
    }

    char answer[4] = {};

    fprintf(stderr, "%s?\n", (*node)->data);
    scanf("%s", answer);

    question:

        if(!strcmp(answer, "no")) {
            DataGuess(tree, &(*node)->left, code_error);
        }
        else if(!strcmp(answer, "yes")) {
            DataGuess(tree, &(*node)->right , code_error);
        }
        else {
            do {
                fprintf(stderr, "please, enter only 'yes' or 'no'\n");
                scanf("%s", answer);
            } while(strcmp(answer, "yes") && strcmp(answer, "no"));

            goto question;
        }
}

void EndOfGame(Tree* tree, Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    fprintf(stderr, "is it %s?\n", node->data);

    char answer[4] = {};
    scanf("%s", answer);

    question:

        if(!strcmp(answer, "no")) {

            char unguess_word[MAX_ANSWER_SIZE] = {};
            char characteristic[MAX_ANSWER_SIZE] = {};

            fprintf(stderr, "who were you thinking of??????\n");
            scanf("%s", unguess_word);
            fprintf(stderr, "write the difference between %s and %s\n", unguess_word, node->data);
            scanf("%s", characteristic);

            Node* NewCharacter = NodeCtor(unguess_word, NULL, NULL, NULL, code_error);
            MY_ASSERT(NewCharacter != NULL, PTR_ERROR);

            Node* NewNode = NodeCtor(characteristic, node, NewCharacter, node->parent, code_error);
            MY_ASSERT(NewNode != NULL, PTR_ERROR);

            if(node == node->parent->left) {
                node->parent->left = NewNode;
            }
            else if(node == node->parent->right) {
                node->parent->right = NewNode;
            }

            NewCharacter->parent = NewNode;
            node->parent = NewNode;

            PrintTree(tree, code_error);
            DotTreeDump(tree, code_error);
            SaveTree(tree, code_error);
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

void GetDefinition(Tree* tree, TreeElem word, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(word != NULL, PTR_ERROR);

    Node* result = NULL;

    WordFind(tree->root, word, &result, code_error);

    if(!result) {
        fprintf(stderr, "word %s not found :(\n", word);
        return;
    }

    Stack_t path_stk = {};
    STACK_CTOR(&path_stk, tree->depth, code_error);

    while(result) {
        StackPush(&path_stk, result, code_error);
        result = result->parent;
    }

    fprintf(stderr, "%s is", word);

    while(path_stk.position >= 2) {
        Node* path_node = 0;

        StackPop(&path_stk, &path_node, code_error);

        if(path_node->left == path_stk.data[path_stk.position - 1]) {
            fprintf(stderr, " no");
        }

        fprintf(stderr, " %s", path_node->data);
    }
    fprintf(stderr, "\n");

    StackDtor(&path_stk, code_error);
}

void WordsCompare(Tree* tree, TreeElem word1, TreeElem word2, int* code_error) {

    MY_ASSERT(tree  != NULL, PTR_ERROR);
    MY_ASSERT(word1 != NULL, PTR_ERROR);
    MY_ASSERT(word2 != NULL, PTR_ERROR);

    Node* result1 = NULL;
    Node* result2 = NULL;

    WordFind(tree->root, word1, &result1, code_error);
    WordFind(tree->root, word2, &result2, code_error);

    Stack_t path1_stk = {};
    Stack_t path2_stk = {};

    STACK_CTOR(&path1_stk, tree->depth, code_error);
    STACK_CTOR(&path2_stk, tree->depth, code_error);

    while(result1) {
        StackPush(&path1_stk, result1, code_error);
        result1 = result1->parent;
    }

    while(result2) {
        StackPush(&path2_stk, result2, code_error);
        result2 = result2->parent;
    }

    fprintf(stderr, "%s and %s are similar in", word1, word2);

    while(path1_stk.position >= 2 && path2_stk.position >= 2) {
        Node* path1_node = 0;
        Node* path2_node = 0;

        StackPop(&path1_stk, &path1_node, code_error);
        StackPop(&path2_stk, &path2_node, code_error);

        if(path1_stk.data[path1_stk.position - 1] != path2_stk.data[path2_stk.position - 1]) {
            fprintf(stderr, " but different in %s", path1_node->data);
        }
        else {
            fprintf(stderr, " %s", path1_node->data);
        }

    }
    fprintf(stderr, "\n");

    StackDtor(&path1_stk, code_error);
    StackDtor(&path2_stk, code_error);
}

void WordFind(Node* node, TreeElem word, Node** result, int* code_error) {

    MY_ASSERT(word != NULL, PTR_ERROR);

    if(!node) {
        return;
    }

    if(!strcmp(word, node->data)) {
        *result = node;
        return;
    }

    if(!(*result)) WordFind(node->left,  word, result, code_error);
    if(!(*result)) WordFind(node->right, word, result, code_error);

}