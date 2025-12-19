//
// Created by XIaokang00010 on 2025/12/13.
//

#include "os.h"

LIBOS_EXPORT int64_t libos_environ(char *** env){
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
