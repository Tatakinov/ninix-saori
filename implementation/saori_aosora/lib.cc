#include "aosora.h"

long aosora_saori_load(char *path, long len) {
    return aosora_load(path, len);
}

int aosora_saori_unload(long id) {
    return aosora_unload(id);
}

char *aosora_saori_request(long id, char *request, long *len) {
    return aosora_request(id, request, len);
}
