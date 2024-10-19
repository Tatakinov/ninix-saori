#include "unix/saori.h"

#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>

#include "header.h"

#define HELPER(a, b) a ## b
#define FUNC(a, b) HELPER(a, b)

std::unique_ptr<Saori> create();

namespace {
    std::unordered_map<int, std::unique_ptr<Saori>> map;
    const long kInstanceMax = 1024;
};

long FUNC(LIBRARY_NAME, _saori_load)(char *path, long len) {
    long id = 0;
    for (long i = 1; i <= kInstanceMax; i++) {
        if (!map.count(i)) {
            id = i;
            break;
        }
    }
    if (!id) {
        return id;
    }
    map.emplace(id, create());
    std::string p(path, len);
    free(path);
    map[id]->load(p);
    return id;
}

int FUNC(LIBRARY_NAME, _saori_unload)(long id) {
    if (map.count(id)) {
        map[id]->unload();
        map.erase(id);
    }
    return 1;
}

char *FUNC(LIBRARY_NAME, _saori_request)(long id, char *request, long *len) {
    if (!map.count(id)) {
        len = 0;
        return NULL;
    }
    std::string r(request, *len);
    free(request);
    saori::Request req = saori::Request::parse(r);
    std::string res = map[id]->request(req);
    *len = res.length();
    return strdup(res.c_str());
}

#undef HELPER
#undef FUNC
