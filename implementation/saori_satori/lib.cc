#include "satori.h"

long satori_saori_load(char *path, long len) {
    return satori_load(path, len);
}

int satori_saori_unload(long id) {
    return satori_unload(id);
}

char *satori_saori_request(long id, char *request, long *len) {
    return satori_request(id, request, len);
}
