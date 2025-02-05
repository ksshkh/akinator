#include "../inc/tree.hpp"
#include "../inc/akinator.hpp"

int main(void) {

    int code_error = 0;
    Tree tree = {};

    TreeCtor(&tree, &code_error);
    ReadTree(&tree, &code_error);
    DotTreeDump(&tree, &code_error);
    AkinatorRun(&tree, &code_error);
    DotTreeDump(&tree, &code_error);
    TreeDtor(&tree, &code_error);

    return 0;
}