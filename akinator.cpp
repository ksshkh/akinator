#include "akinator.hpp"
#include "utils.hpp"

void AkinatorRun(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    char answer = 0;

    printf("hi, let's play the game\n");

    while(true) {
        printf("Press\nq: for quit\ng: for guess character\nd: for get definition\nc: for compare two characters\n");

        if(scanf("%c", &answer) != 1) {
            printf("pls, use only letters q, g, d, c\n");
            CleanBuffer();
            continue;
        }

        switch(answer) {
            case 'q': {
                printf("bye bye\n");
                return;
            }
            case 'g': {
                DataGuess(tree, &(tree->root), code_error);
                CleanBuffer();
                break;
            }
            case 'd': {
                printf("enter name of a character\n");
                CleanBuffer();

                char* character = GetString(stdin, code_error);
                MY_ASSERT(character != NULL, PTR_ERROR);

                GetDefinition(tree, character, code_error);
                break;
            }
            case 'c': {
                /*printf("enter name of the first character\n");

                char character1 = 0;
                scanf("%s", &character1);
                printf("!%s!\n", &character1);

                printf("enter name of the second character\n");

                char character2 = 0;
                scanf("%s", &character2);
                printf("!%s!\n", &character2);

                WordsCompare(tree, &character1, &character2, code_error);
                break;*/
            }
            default:
                printf("Retry now\n");
        }
    }
}

void DataGuess(Tree* tree, Node** node, int* code_error) {

    if((*node)->left == NULL || (*node)->right == NULL) {
        EndOfGame(tree, *node, code_error);
        return;
    }

    char* answer = GetString(stdin, code_error);
    MY_ASSERT(answer != NULL, PTR_ERROR);

    printf("%s?\n", (*node)->data);

    while(strcmp(answer, "no") && strcmp(answer, "yes")) {
        printf("please, enter only 'yes' or 'no'\n");
        answer = GetString(stdin, code_error);
        MY_ASSERT(answer != NULL, PTR_ERROR);
    }

    if(!strcmp(answer, "no")) {
        DataGuess(tree, &(*node)->left, code_error);
    }
    else if(!strcmp(answer, "yes")) {
        DataGuess(tree, &(*node)->right , code_error);
    }

    free(answer);
}

void EndOfGame(Tree* tree, Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    printf("is it %s?\n", node->data);

    char* answer = GetString(stdin, code_error);
    MY_ASSERT(answer != NULL, PTR_ERROR);

    while(strcmp(answer, "no") && strcmp(answer, "yes")) {
        printf("please, enter only 'yes' or 'no'\n");
        answer = GetString(stdin, code_error);
        MY_ASSERT(answer != NULL, PTR_ERROR);
    }

    if(!strcmp(answer, "no")) {

        printf("who were you thinking of??????\n");

        char* unguess_word = GetString(stdin, code_error);
        MY_ASSERT(unguess_word != NULL, PTR_ERROR);

        printf("write the difference between %s and %s\n", unguess_word, node->data);

        char* characteristic = GetString(stdin, code_error);
        MY_ASSERT(characteristic != NULL, PTR_ERROR);

        Node* new_character = NodeCtor(unguess_word, NULL, NULL, NULL, code_error);
        MY_ASSERT(new_character != NULL, PTR_ERROR);

        Node* new_node = NodeCtor(characteristic, node, new_character, node->parent, code_error);
        MY_ASSERT(new_node != NULL, PTR_ERROR);

        if(node == node->parent->left) {
            node->parent->left = new_node;
        }
        else if(node == node->parent->right) {
            node->parent->right = new_node;
        }

        new_character->parent = new_node;
        node->parent = new_node;

        PrintTree(tree, code_error);
        DotTreeDump(tree, code_error);
        // SaveTree(tree, code_error);
        free(unguess_word);
        free(characteristic);
    }
    else if(!strcmp(answer, "yes")) {
        printf("hype\n");
    }

    free(answer);
}

void GetDefinition(Tree* tree, TreeElem word, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(word != NULL, PTR_ERROR);

    Node* result = NULL;

    WordFind(tree->root, word, &result, code_error);

    if(!result) {
        printf("word %s not found :(\n", word);
        return;
    }

    Stack_t path_stk = {};
    STACK_CTOR(&path_stk, tree->depth, code_error);

    while(result) {
        StackPush(&path_stk, result, code_error);
        result = result->parent;
    }

    printf("%s is", word);

    while(path_stk.position >= 2) {
        Node* path_node = 0;

        StackPop(&path_stk, &path_node, code_error);

        if(path_node->left == path_stk.data[path_stk.position - 1]) {
            printf(" no");
        }

        printf(" %s", path_node->data);
    }

    printf("\n");

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

    printf("%s and %s are similar in", word1, word2);

    while(path1_stk.position >= 2 && path2_stk.position >= 2) {
        Node* path1_node = 0;
        Node* path2_node = 0;

        StackPop(&path1_stk, &path1_node, code_error);
        StackPop(&path2_stk, &path2_node, code_error);

        if(path1_stk.data[path1_stk.position - 1] != path2_stk.data[path2_stk.position - 1]) {
            printf(" but different in %s", path1_node->data);
        }
        else {
            printf(" %s", path1_node->data);
        }

    }
    printf("\n");

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