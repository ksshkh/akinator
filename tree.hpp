#ifndef TREE_HPP
#define TREE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "errors.hpp"

typedef char* TreeElem;

struct Node {
    TreeElem data = 0;

    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;
};

struct Tree {
    Node* root = NULL;
    size_t depth = 0;

    char* data_base = 0;
    long int size_data_base = 0;
};

void TreeCtor(Tree* tree, int* code_error);

Node* NodeCtor(TreeElem data, Node* left, Node* right, Node* parent, int* code_error);

// void DataInsert(Node** node, TreeElem data, int* code_error);

void DotTreeDump(Tree* tree, int* code_error);

void TreeDtor(Tree* tree, int* code_error);

void PrintNode(Node* node, FILE* stream);

void FreeNode(Node* node, int* code_error);

void GraphCreate(void);

void HtmlDump(int *code_error);

void PrintTree(Tree* tree, int* code_error);

void PreorderPrinting(Node* node, FILE* stream, int* code_error);

void ReadTree(Tree* tree, int* code_error);

Node* ReadNode(Tree* tree, Node* node, Node* parent, int* code_error);

void GetTreeDepth(Tree* tree, int* code_error);

void SaveTree(Tree* tree, int* code_error);

#endif // TREE_HPP