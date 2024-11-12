#include "errors.hpp"

static const char* errors_names[] = {"NO_ERROR",
                                     "PTR_ERROR",
                                     "SIZE_ERROR",
                                     "FILE_ERROR",
                                     "READ_ERROR",
                                     "NO_DATA",
                                     "LOST_NODE"};

void ErrorsPrint(FILE* stream, int* code_error) {
    for (int i = 0; i < N_ERROR; i++) {
        if (*code_error & (1 << i)) {
            fprintf(stream, "\x1b[31mERROR: %s\x1b[0m\n", errors_names[i]);
        }
    }
}