#include "tree.hpp"
#include "akinator.hpp"

int main(void) {

    int code_error = 0;
    Tree tree = {};

    TreeCtor(&tree, &code_error);

    // DataInsert(&tree.root, "mipt", &code_error);
    // DataInsert(&tree.root, "rt", &code_error);
    // DataInsert(&tree.root, "os'", &code_error);
    // DataInsert(&tree.root, "fivt", &code_error);
    // DataInsert(&tree.root, "hz", &code_error);
    ReadTree(&tree, &code_error);
    DotTreeDump(&tree, &code_error);
    // Node* res = NULL;
    // WordFind(tree.root, "kx", &res, &code_error);
    // fprintf(stderr, "<%p>\n", res);
    // GetDefinition(&tree, "polina", &code_error);
    // WordsCompare(&tree, "kx", "polina", &code_error);
    AkinatorRun(&tree, &code_error);
    // DotTreeDump(&tree, &code_error);

    // DataGuess(&tree.root, &code_error);

    // PrintTree(&tree, &code_error);
    // fprintf(stderr, "depth: %ld\n", tree.depth);

    return 0;
}