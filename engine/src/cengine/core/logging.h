#pragma once

#include <stdio.h>

#include "defines.h"

#define LOG_TRACE(fmt, ...)   \
    printf("[TRACE]: ");      \
    printf(fmt, __VA_ARGS__); \
    printf("\n");

#define LOG_INFO(fmt, ...)    \
    printf("[INFO]: ");       \
    printf(fmt, __VA_ARGS__); \
    printf("\n");

#define LOG_WARN(fmt, ...)    \
    printf("[WARN]: ");       \
    printf(fmt, __VA_ARGS__); \
    printf("\n");

#define LOG_ERROR(fmt, ...)   \
    printf("[ERROR]: ");      \
    printf(fmt, __VA_ARGS__); \
    printf("\n");

#define LOG_CRITICAL(fmt, ...) \
    printf("[CRITICAL]: ");    \
    printf(fmt, __VA_ARGS__);  \
    printf("\n");

#define ASSERT(cond)                                                                                   \
    {                                                                                                  \
        if (cond) {                                                                                    \
        } else {                                                                                       \
            LOG_CRITICAL("Assertion failed: '%s' in file '%s' on line %i", #cond, __FILE__, __LINE__); \
            __debugbreak();                                                                            \
        }                                                                                              \
    }

#define ASSERT_MSG(cond, msg)                                                                          \
    {                                                                                                  \
        if (cond) {                                                                                    \
        } else {                                                                                       \
            LOG_CRITICAL("Assertion failed: '%s' in file '%s' on line %i", #cond, __FILE__, __LINE__); \
            LOG_ERROR("'%s'", msg)                                                                     \
            __debugbreak();                                                                            \
        }                                                                                              \
    }