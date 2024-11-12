#include "tree.hpp"

int main(void) {

    int code_error = 0;
    Tree tree = {};

    DataInsert(&tree.root, "mipt", &code_error);
    DataInsert(&tree.root, "rt", &code_error);
    DataInsert(&tree.root, "hz", &code_error);

    DotTreeDump(&tree, &code_error);

    DataFind(&tree.root, &code_error);

    return 0;
}