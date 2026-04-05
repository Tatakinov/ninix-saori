#include <cstring>

#include "kawari.h"

extern "C" long kawari_saori_load(char *path, long len) {
    return so_create(path, len);
}

extern "C" int kawari_saori_unload(long id) {
    return so_dispose(id);
}

extern "C" char *kawari_saori_request(long id, char *request, long *len) {
    // malloc/freeとnew/deleteは混ぜない
    char *p = so_request(id, request, len);
    char *dup = strndup(p, *len);
    so_free(id, p);
    return dup;
}
