#ifndef CENGINE_MACROS_H
#define CENGINE_MACROS_H

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define CENGINE_IS_LINUX 0
#define CENGINE_IS_WINDOWS 1
#elif defined(unix) || defined(__unix) || defined(__unix__)
#define CENGINE_IS_LINUX 1
#define CENGINE_IS_WINDOWS 0
#endif

#endif
