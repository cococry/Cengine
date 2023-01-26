#pragma once

#include "../core/defines.h"

char* get_file_content(const char* filename);

float get_random_float(float max);

char* strremove(char* str, const char* sub);

char* trimstr(char* str, char endc);

i32 get_index_of_char_in_str(char* str, char c);
