#ifndef TREE_HPP
#define TREE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "errors.hpp"

typedef char* TreeElem;

struct Node {
    TreeElem data = 0;

    Node* left = NULL;
    Node* right = NULL;
};

struct Tree {
    Node* root = NULL;
    size_t tree_size = 0;
};

Node* NodeCtor(TreeElem data, Node* left, Node* right, int* code_error);

void DataInsert(Node** node, TreeElem data, int* code_error);

void DataFind(Node** node, int* code_error);

void EndOfGame(Node* node, int* code_error);

void DotTreeDump(Tree* tree, int* code_error);

void TreeDtor(Tree* tree, int* code_error);

void PrintNode(Node* node, FILE* stream);

void GraphCreate(void);

long int count_size_file(FILE* program, int* code_error);

void HtmlDump(int *code_error);

#endif // TREE_HPP