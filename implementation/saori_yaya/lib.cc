#include "yaya.h"

extern "C" long yaya_saori_load(char *path, long len) {
    return multi_load(path, len);
}

extern "C" int yaya_saori_unload(long id) {
    return multi_unload(id);
}

extern "C" char *yaya_saori_request(long id, char *request, long *len) {
    return multi_request(id, request, len);
}
