#ifndef AKINATOR_HPP
#define AKINATOR_HPP

#include "tree.hpp"
#include "stack.hpp"

const int MAX_ANSWER_SIZE = 20;

void AkinatorRun(Tree* tree, int* code_error);

void DataGuess(Tree* tree, Node** node, int* code_error);

void EndOfGame(Tree* tree, Node* node, int* code_error);

void GetDefinition(Tree* tree, TreeElem word, int* code_error);

void WordsCompare(Tree* tree, TreeElem word1, TreeElem word2, int* code_error);

void WordFind(Node* node, TreeElem word, Node** result, int* code_error);

#endif // AKINATOR_HPP