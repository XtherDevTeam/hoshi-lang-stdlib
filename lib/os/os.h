//
// Created by XIaokang00010 on 2025/12/13.
//

#ifndef HOSHI_LANG_STDLIB_OS_OS_H
#define HOSHI_LANG_STDLIB_OS_OS_H

#include <cstdint>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else

#include <unistd.h>

extern char **environ;

#endif

#define LIBOS_EXPORT extern "C"

LIBOS_EXPORT int64_t libos_environ(char ***);

LIBOS_EXPORT uint64_t libos_helper_dereference_ptr(void *ptr);

LIBOS_EXPORT int64_t libos_system(char *ptr);

LIBOS_EXPORT void libos_exit(int64_t status);
#endif // HOSHI_LANG_STDLIB_OS_OS_H