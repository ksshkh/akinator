#include "tree.hpp"

#define NODE_COLOR         "\"#EC9594\""
#define NODE_BORDER_COLOR  "\"#FEE8D6\""
#define BACKGROUND_COLOR   "\"#0F0000\""

static const char* DOT_FILE_NAME   = "./debug/tree.dot";
static const char* DEBUG_FILE_NAME = "./debug/tree_dump.txt";
static const char* IMAGE_NAME      = "./debug/tree_image.svg";
static const char* HTML_FILE_NAME  = "./debug/tree.html";

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

    tree->tree_size = 0;
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

long int count_size_file(FILE* program, int* code_error) {

    MY_ASSERT(program != NULL, FILE_ERROR);

    struct stat file_info = {};
    fstat(fileno(program), &file_info);

    return file_info.st_size;
}

void GraphCreate(void) {

    char command[] = "dot -Tsvg /home/ksenia/huawei/akinator/debug/tree.dot -o /home/ksenia/huawei/akinator/debug/tree_image.svg";
    system(command);
}

void HtmlDump(int *code_error) {

    FILE* image = fopen(IMAGE_NAME, "r");
    MY_ASSERT(image != NULL, FILE_ERROR);

    FILE* html = fopen(HTML_FILE_NAME, "a");
    MY_ASSERT(image != NULL, FILE_ERROR);

    size_t image_size = count_size_file(image, code_error);

    char *image_data = (char*)calloc(image_size, sizeof(char));
    MY_ASSERT(image_data != NULL, FILE_ERROR);

    fread(image_data, sizeof(char), image_size, image);

    fprintf(html, "%s\n", image_data);

    MY_ASSERT(fclose(image) == 0, FILE_ERROR);
    MY_ASSERT(fclose(html) == 0, FILE_ERROR);
}