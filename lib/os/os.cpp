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
