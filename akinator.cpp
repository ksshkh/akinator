#include "akinator.hpp"
#include "utils.hpp"

void AkinatorRun(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    char mode = 0;

    printf("hi, let's play the game\n");

    while(true) {
        printf("Press\nq: for quit\ng: for guess character\nd: for get definition\nc: for compare two characters\n");

        scanf("%c", &mode);

        switch(mode) {
            case QUITE: {
                printf("bye bye\n");
                return;
            }
            case GUESS: {
                DataGuess(tree, &(tree->root), code_error);
                break;
            }
            case DEFENITION: {
                printf("enter name of a character\n");
                CleanBuffer();

                char* character = GetString(stdin, code_error);
                MY_ASSERT(character != NULL, PTR_ERROR);

                GetDefinition(tree, character, code_error);

                free(character);
                break;
            }
            case COMPARE: {
                printf("enter name of the first character\n");
                CleanBuffer();

                char* first_character = GetString(stdin, code_error);
                MY_ASSERT(first_character != NULL, PTR_ERROR);

                printf("enter name of the second character\n");

                char* second_character = GetString(stdin, code_error);
                MY_ASSERT(second_character != NULL, PTR_ERROR);

                WordsCompare(tree, first_character, second_character, code_error);

                free(first_character);
                free(second_character);
                break;
            }
            default:
                printf("wrong input!!\n");
                CleanBuffer();
        }
    }
}

void DataGuess(Tree* tree, Node** node, int* code_error) {

    if((*node)->left == NULL || (*node)->right == NULL) {
        EndOfGuess(tree, *node, code_error);
        return;
    }

    char* answer = GetString(stdin, code_error);
    MY_ASSERT(answer != NULL, PTR_ERROR);

    printf("%s?\n", (*node)->data);

    CHECK_WRONG_AHSWER(answer);

    if(!strcmp(answer, "no")) {
        DataGuess(tree, &(*node)->left, code_error);
    }
    else if(!strcmp(answer, "yes")) {
        DataGuess(tree, &(*node)->right , code_error);
    }

    free(answer);
}

void EndOfGuess(Tree* tree, Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    printf("is it %s?\n", node->data);

    char* answer = GetString(stdin, code_error);
    MY_ASSERT(answer != NULL, PTR_ERROR);

    CHECK_WRONG_AHSWER(answer);

    if(!strcmp(answer, "no")) {

        printf("who were you thinking of??????\n");

        char* unguess_word = GetString(stdin, code_error);
        MY_ASSERT(unguess_word != NULL, PTR_ERROR);

        printf("write the difference between %s and %s\n", unguess_word, node->data);

        char* characteristic = GetString(stdin, code_error);
        MY_ASSERT(characteristic != NULL, PTR_ERROR);

        AddNewNode(characteristic, unguess_word, node, code_error);

        PrintTree(tree, code_error);
        DotTreeDump(tree, code_error);
        SaveTree(tree, code_error);

        free(unguess_word);
        free(characteristic);
    }
    else if(!strcmp(answer, "yes")) {
        printf("hype\n");
    }

    free(answer);
}

void AddNewNode(char* characteristic, char* unguess_word, Node* node, int* code_error) {

    MY_ASSERT(characteristic != NULL, PTR_ERROR);
    MY_ASSERT(unguess_word   != NULL, PTR_ERROR);
    MY_ASSERT(node           != NULL, PTR_ERROR);

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
}

void GetDefinition(Tree* tree, TreeElem word, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(word != NULL, PTR_ERROR);

    Node* result = NULL;

    WordFind(tree->root, word, &result, code_error);

    WORD_NOT_FOUND(result, word)

    Stack_t* path_stk = GetTreePath(tree, result, code_error);

    PrintDefinition(path_stk, word, code_error);

    printf("\n");

    StackDtor(path_stk, code_error);
}

void PrintDefinition(Stack_t* stk, TreeElem word, int* code_error) {

    MY_ASSERT(stk != NULL, PTR_ERROR);

    printf("%s is", word);

    if(stk->position < 2) {
        printf(" %s", word);
        return;
    }

    while(stk->position >= 2) {
        Node* path_node = 0;

        StackPop(stk, &path_node, code_error);

        if(path_node->left == stk->data[stk->position - 1]) {
            printf(" no");
        }

        printf(" %s", path_node->data);
    }
}

void WordsCompare(Tree* tree, TreeElem word1, TreeElem word2, int* code_error) {

    MY_ASSERT(tree  != NULL, PTR_ERROR);
    MY_ASSERT(word1 != NULL, PTR_ERROR);
    MY_ASSERT(word2 != NULL, PTR_ERROR);

    Node* result1 = NULL;
    Node* result2 = NULL;

    WordFind(tree->root, word1, &result1, code_error);
    WordFind(tree->root, word2, &result2, code_error);

    WORD_NOT_FOUND(result1, word1)
    WORD_NOT_FOUND(result2, word2);

    printf("%s and %s are", word1, word2);

    if(!strcmp(word1, word2)) {
        printf(" same\n");
        return;
    }

    Stack_t* path1_stk = GetTreePath(tree, result1, code_error);
    Stack_t* path2_stk = GetTreePath(tree, result2, code_error);

    printf(" similar in");

    while(path1_stk->position >= 2 && path2_stk->position >= 2) {
        Node* path1_node = 0;
        Node* path2_node = 0;

        StackPop(path1_stk, &path1_node, code_error);
        StackPop(path2_stk, &path2_node, code_error);

        if(path1_stk->data[path1_stk->position - 1] != path2_stk->data[path2_stk->position - 1]) {
            printf("\nbut ");
            PrintDefinition(path1_stk, word1, code_error);

            printf(" and ");
            PrintDefinition(path2_stk, word2, code_error);
        }
        else {
            printf(" %s", path1_node->data);
        }
    }
    printf("\n");

    StackDtor(path1_stk, code_error);
    StackDtor(path2_stk, code_error);
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

Stack_t* GetTreePath(Tree* tree, Node* node, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(node != NULL, PTR_ERROR);

    Stack_t* stk = (Stack_t*)calloc(1, sizeof(Stack_t));
    MY_ASSERT(stk != NULL, PTR_ERROR);

    STACK_CTOR(stk, tree->depth, code_error);

    while(node) {
        StackPush(stk, node, code_error);
        node = node->parent;
    }

    return stk;
}