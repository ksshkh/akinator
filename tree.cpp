#include "tree.hpp"
#include "utils.hpp"

#define NODE_COLOR         "\"#C0261C\""
#define NODE_BORDER_COLOR  "\"#FFBE20\""
#define BACKGROUND_COLOR   "\"#4984F9\""

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

Node* NodeCtor(TreeElem data, Node* left, Node* right, Node* parent, int* code_error) {

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    MY_ASSERT(new_node != NULL, PTR_ERROR);

    new_node->data = data;
    new_node->left = left;
    new_node->right = right;
    new_node->parent = parent;

    return new_node;
}

void AddNewNode(Node* node, TreeElem data, Side side, int* code_error) {

    MY_ASSERT(data != NULL, PTR_ERROR);
    MY_ASSERT(node != NULL, PTR_ERROR);

    if(side == LEFT) {
        Node* new_node = NodeCtor(data, node->left, NULL, node, code_error);
        MY_ASSERT(new_node != NULL, PTR_ERROR);

        node->left = new_node;
    }
    else if(side == RIGHT) {
        Node* new_node = NodeCtor(data, NULL, node->right, node, code_error);
        MY_ASSERT(new_node != NULL, PTR_ERROR);

        node->right = new_node;
    }

}

void TreeDtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    FreeNode(tree->root, code_error);
    tree->root = NULL;

    free(tree->data_base);
    tree->data_base = NULL;

    tree->depth = 0;
    tree->size_data_base = 0;

}

void FreeNode(Node* node, int* code_error) {

    if(!node) return;

    FreeNode(node->left, code_error);
    FreeNode(node->right, code_error);
    free(node);

}

void DotTreeDump(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    FILE* dot_file = fopen(DOT_FILE_NAME, "w");
    MY_ASSERT(dot_file != NULL, FILE_ERROR);

    fprintf(dot_file, "digraph Tree {\n");
    fprintf(dot_file, "\trankdir = TB;\n");
    fprintf(dot_file, "\tnode [shape = record];\n");
    fprintf(dot_file, "\tedge [color = " NODE_BORDER_COLOR "];\n");
    fprintf(dot_file, "\tbgcolor = " BACKGROUND_COLOR ";\n");

    PrintNode(tree->root, dot_file);

    fprintf(dot_file, "}\n");

    MY_ASSERT(fclose(dot_file) == 0, FILE_ERROR);

    GraphCreate();
    HtmlDump(code_error);
}

void PrintNode(Node* node, FILE* stream) {

    if(!node) return;

    fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_COLOR ", label = \"{indx: %p | value: %s | parent: %p | { left: %p | right: %p}}\"];\n",
            node, node, node->data, node->parent, node->left, node->right);

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

    char* copy_data_base = tree->data_base;

    tree->root = ReadNode(tree, tree->root, NULL, code_error);

    tree->data_base = copy_data_base;
}

Node* ReadNode(Tree* tree, Node* node, Node* parent, int* code_error) {

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

    node = NodeCtor(data, NULL, NULL, parent, code_error);

    node->left = ReadNode(tree, node->left, node, code_error);

    while(*(tree->data_base) == ')' || isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    node->right = ReadNode(tree, node->right, node, code_error);

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

#undef NODE_COLOR
#undef NODE_BORDER_COLOR
#undef BACKGROUND_COLOR