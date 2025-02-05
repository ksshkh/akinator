#ifndef AKINATOR_HPP
#define AKINATOR_HPP

#include "tree.hpp"
#include "stack.hpp"

enum Modes {
    QUITE      = 'q',
    GUESS      = 'g',
    DEFENITION = 'd',
    COMPARE    = 'c',
    SAVE       = 's',
};

void AkinatorRun(Tree* tree, int* code_error);

void DataGuess(Tree* tree, Node** node, int* code_error);

void EndOfGuess(Node* node, int* code_error);

void GetDefinition(Tree* tree, const TreeElem word, int* code_error);

void PrintDefinition(Stack_t* stk, const TreeElem word, int* code_error);

void WordsCompare(Tree* tree, const TreeElem word1, const TreeElem word2, int* code_error);

void WordFind(Node* node, const TreeElem word, Node** result, int* code_error);

Stack_t* GetTreePath(Tree* tree, Node* node, int* code_error);

#endif // AKINATOR_HPP