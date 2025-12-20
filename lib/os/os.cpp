//
// Created by XIaokang00010 on 2025/12/13.
//

#include "os.h"

#ifdef _WIN32
#include <windows.h>
#include <io.h>

LIBOS_EXPORT int setenv(const char* name, const char* value, int overwrite) {
    _putenv_s(name, value);
    return 0;
}

LIBOS_EXPORT int unsetenv(const char* name) {
    _putenv_s(name, "");
    return 0;
}

LIBOS_EXPORT int putenv(const char* name) {
    return _putenv(name);
}
#else
#include <sys/stat.h>
#endif

LIBOS_EXPORT int64_t libos_environ(char *** env) {
    *env = environ;
    int64_t count = 0;
    while (environ[count]) {
        count++;
    }
    return count;
}

LIBOS_EXPORT uint64_t libos_helper_dereference_ptr(void *ptr) {
    return (uint64_t)*(uint64_t *)ptr;
}

int64_t libos_system(char *ptr) {
    return system(ptr);
}

void libos_exit(int64_t status) {
    exit(status);
}

void* libos_popen(const char *cmd, const char *mode) {
#ifdef _WIN32
    return (void*)_popen(cmd, mode);
#else
    return (void*)popen(cmd, mode);
#endif
}

int64_t libos_pclose(void* file) {
#ifdef _WIN32
    return _pclose((FILE *)file);
#else
    return pclose((FILE *)file);
#endif
}

int64_t libos_chmod(const char *path, int64_t mode) {
#ifdef _WIN32
    return _chmod(path, mode);
#else
    return chmod(path, mode);
#endif
}
