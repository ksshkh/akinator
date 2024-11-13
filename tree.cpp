#include "tree.hpp"
#include "utils.hpp"
#define NODE_COLOR         "\"#EC9594\""
#define NODE_BORDER_COLOR  "\"#FEE8D6\""
#define BACKGROUND_COLOR   "\"#0F0000\""

static const char* DOT_FILE_NAME   = "./debug/tree.dot";
static const char* DEBUG_FILE_NAME = "./debug/tree_dump.txt";
static const char* IMAGE_NAME      = "./debug/tree_image.svg";
static const char* HTML_FILE_NAME  = "./debug/tree.html";
static const char* AKINATOR_TREE_FILE  = "./akinator.txt";

void TreeCtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    tree->depth = 0;
    tree->root = NULL;

    tree->data_base = ReadInBuff(AKINATOR_TREE_FILE, &(tree->size_data_base), code_error);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    GetTreeDepth(tree, code_error);
}

Node* NodeCtor(TreeElem data, Node* left, Node* right, int* code_error) {

    Node* NewNode = (Node*)calloc(1, sizeof(Node));
    MY_ASSERT(NewNode != NULL, PTR_ERROR);

    NewNode->data = data;
    NewNode->left = left;
    NewNode->right = right;

    return NewNode;
}

void DataInsert(Node** node, TreeElem value, int* code_error) {

    if(*node == NULL) {
        *node = NodeCtor(value, NULL, NULL, code_error);
        MY_ASSERT(node != NULL, PTR_ERROR);

        return;
    }

    char answer[4] = {};

    fprintf(stderr, "is %s %s?\n", value, (*node)->data);
    scanf("%s", answer);

    if(!strcmp(answer, "no")) {
        DataInsert(&(*node)->left, value, code_error);
    }
    else if(!strcmp(answer, "yes")) {
        DataInsert(&(*node)->right, value, code_error);
    }
    else {
        fprintf(stderr, "please, enter only 'yes' or 'no'\n");
    }

}

void DataFind(Node** node, int* code_error) {

    if((*node)->left == NULL || (*node)->right == NULL) {
        EndOfGame(*node, code_error);
        return;
    }

    char answer[4] = {};

    fprintf(stderr, "%s?\n", (*node)->data);
    scanf("%s", answer);

    if(!strcmp(answer, "no")) {
        DataFind(&(*node)->left, code_error);
    }
    else if(!strcmp(answer, "yes")) {
        DataFind(&(*node)->right , code_error);
    }
    else {
        fprintf(stderr, "please, enter only 'yes' or 'no'\n");
    }
}

void EndOfGame(Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    fprintf(stderr, "is it %s?\n", node->data);

    char answer[4] = {};
    scanf("%s", answer);

    if(!strcmp(answer, "no")) {
        fprintf(stderr, "ne hype\n");
    }
    else if(!strcmp(answer, "yes")) {
        fprintf(stderr, "hype\n");
    }
    else {
        fprintf(stderr, "please, enter only 'yes' or 'no'\n");
    }
}

void TreeDtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    free(tree->root);
    tree->root = NULL;

    free(tree->data_base);
    tree->data_base = NULL;

    tree->depth = 0;
    tree->size_data_base = 0;

}

void DotTreeDump(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    FILE* dot_file = fopen(DOT_FILE_NAME, "w");
    MY_ASSERT(dot_file != NULL, FILE_ERROR);

    fprintf(dot_file, "digraph Tree {\n");
    fprintf(dot_file, "\trankdir = TB;\n");
    fprintf(dot_file, "\tnode [shape = record];\n");
    fprintf(dot_file, "\tedge[color = " NODE_BORDER_COLOR "];\n");
    fprintf(dot_file, "\tbgcolor = " BACKGROUND_COLOR ";\n");

    PrintNode(tree->root, dot_file);

    fprintf(dot_file, "}\n");

    MY_ASSERT(fclose(dot_file) == 0, FILE_ERROR);

    GraphCreate();
    HtmlDump(code_error);
}

void PrintNode(Node* node, FILE* stream) {

    if(!node) return;

    fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_COLOR ", label = \"{indx: %p | value: %s | { left: %p | right: %p}}\"];\n",
            node, node, node->data, node->left, node->right);

    if(node->left) {
        fprintf(stream, "\t\tnode%p -> node%p\n", node, node->left);
        PrintNode(node->left,  stream);
    }

    if(node->right) {
        fprintf(stream, "\t\tnode%p -> node%p\n", node, node->right);
        PrintNode(node->right, stream);
    }

}

void GraphCreate(void) {

    char command[] = "dot -Tsvg /home/ksenia/huawei/akinator/debug/tree.dot -o /home/ksenia/huawei/akinator/debug/tree_image.svg";
    system(command);
}

void HtmlDump(int *code_error) {

    FILE* html = fopen(HTML_FILE_NAME, "a");
    MY_ASSERT(html != NULL, FILE_ERROR);

    long int image_size = 0;

    char *image_data = ReadInBuff(IMAGE_NAME, &image_size, code_error);
    MY_ASSERT(image_data != NULL, FILE_ERROR);

    fprintf(html, "%s\n", image_data);

    MY_ASSERT(fclose(html) == 0, FILE_ERROR);
}

void PrintTree(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, FILE_ERROR);

    FILE* printout = fopen(AKINATOR_TREE_FILE, "w");
    MY_ASSERT(printout != NULL, FILE_ERROR);

    PreorderPrinting(tree->root, printout, code_error);

    MY_ASSERT(fclose(printout) == 0, FILE_ERROR);
}

void PreorderPrinting(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    if(!node) {
        fprintf(stream, " nil ");
        return;
    }

    fprintf(stream, "(");
    fprintf(stream, "<%s>", node->data);

    PreorderPrinting(node->left, stream, code_error);
    PreorderPrinting(node->right, stream, code_error);

    fprintf(stream, ")");
}

void ReadTree(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, FILE_ERROR);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    tree->root = ReadNode(tree, tree->root, code_error);
}

Node* ReadNode(Tree* tree, Node* node, int* code_error) {

    while(isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
        tree->data_base++;
    }

    if(*(tree->data_base) == ')') {
        return node;
    }

    if(!strncmp(tree->data_base, "nil", 3)) {
        tree->data_base += 3;
        return node;
    }

    while(*(tree->data_base) != '<') {
        tree->data_base++;
    }

    char* data = ++(tree->data_base);

    while(*(tree->data_base) != '>') {
        tree->data_base++;
    }

    *(tree->data_base) = '\0';

    node = NodeCtor(data, NULL, NULL, code_error);

    node->left = ReadNode(tree, node->left, code_error);

    while(*(tree->data_base) == ')' || isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    node->right = ReadNode(tree, node->right, code_error);

    return node;
}

void GetTreeDepth(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, FILE_ERROR);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    size_t curtain_depth = 0;

    for(long int i = 0; i < tree->size_data_base; i++) {

        if     (tree->data_base[i] == '(') curtain_depth++;
        else if(tree->data_base[i] == ')') curtain_depth--;

        tree->depth = (curtain_depth > tree->depth) ? curtain_depth : tree->depth;
    }
}
// yes yes yes yes no no
// void ReadNode(Node** node, )