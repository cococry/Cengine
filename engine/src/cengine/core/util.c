#include "util.h"

#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

float get_random_float(float max) {
    return (float)rand() / (float)(RAND_MAX / max);
}

char* strremove(char* str, const char* sub) {
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            memmove(q, p, r - p);
            q += r - p;
        }
        memmove(q, p, strlen(p) + 1);
    }
    return str;
}
char* trimstr(char* str, char endc) {
    char* ret = str;
    for (u32 i = 0; i < strlen(ret); i++) {
        if (ret[i] == endc) {
            ret[i] = '\0';
        }
    }
    return ret;
}

i32 get_index_of_char_in_str(char* str, char c) {
    char* ptr;
    u32 ret;

    ptr = strchr(str, c);
    if (ptr == nullptr) {
        return -1;
    }

    ret = ptr - str;

    ASSERT(str[ret] == c);

    return ret;
}