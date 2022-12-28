#include "util.h"

#include "logging.h"

#include <stdio.h>
#include <stdlib.h>

char* get_file_content(const char* filename) {
    char* buffer = 0;
    long length;
    FILE* f = fopen(filename, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);

        buffer[length] = '\0';
    }

    ASSERT_MSG(buffer != nullptr, "Failed to load file.");

    return buffer;
}