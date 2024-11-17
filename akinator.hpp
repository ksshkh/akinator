#ifndef AKINATOR_HPP
#define AKINATOR_HPP

#include "tree.hpp"
#include "stack.hpp"

enum Modes {
    QUITE      = 'q',
    GUESS      = 'g',
    DEFENITION = 'd',
    COMPARE    = 'c',
};

#define WORD_NOT_FOUND(result, word)    if(!result) {                                           \
                                            printf("word %s was not found :(\n", word);         \
                                            return;                                             \
                                        }                                                       \

#define CHECK_WRONG_AHSWER(answer)      while(strcmp(answer, "no") && strcmp(answer, "yes")) {  \
                                            printf("please, enter only 'yes' or 'no'\n");       \
                                            answer = GetString(stdin, code_error);              \
                                            MY_ASSERT(answer != NULL, PTR_ERROR);               \
                                        }                                                       \

void AkinatorRun(Tree* tree, int* code_error);

void DataGuess(Tree* tree, Node** node, int* code_error);

void EndOfGuess(Tree* tree, Node* node, int* code_error);

void AddNewNode(char* characteristic, char* unguess_word, Node* node, int* code_error);

void GetDefinition(Tree* tree, TreeElem word, int* code_error);

void PrintDefinition(Stack_t* stk, TreeElem word, int* code_error);

void WordsCompare(Tree* tree, TreeElem word1, TreeElem word2, int* code_error);

void WordFind(Node* node, TreeElem word, Node** result, int* code_error);

Stack_t* GetTreePath(Tree* tree, Node* node, int* code_error);

#endif // AKINATOR_HPP