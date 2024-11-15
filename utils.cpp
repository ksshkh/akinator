#include "utils.hpp"

long int count_size_file(FILE* program, int* code_error) {

    MY_ASSERT(program != NULL, FILE_ERROR);

    struct stat file_info = {};
    fstat(fileno(program), &file_info);

    return file_info.st_size;
}

char* ReadInBuff(const char* filename, long int* stream_size, int* code_error) {

    FILE* stream = fopen(filename, "r");
    MY_ASSERT(stream != NULL, FILE_ERROR);

    *stream_size = count_size_file(stream, code_error);

    char *buffer = (char*)calloc(*stream_size, sizeof(char));
    MY_ASSERT(buffer != NULL, FILE_ERROR);

    fread(buffer, sizeof(char), *stream_size, stream);

    MY_ASSERT(fclose(stream) == 0, FILE_ERROR);

    return buffer;
}

void CleanBuffer(void) {
    int c = 0;
    do {
        c = getchar();
    } while(c != '\n' && c != EOF);
}