#ifndef OCTO_FISHSTICK_HELPERS_MACRO
#define OCTO_FISHSTICK_HELPERS_MACRO

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

_Noreturn void panic(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "[OCTO_PANIC]: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);

    abort();
}

#endif
