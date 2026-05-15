#include "windows/saori.h"

#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

#include "header.h"

std::unique_ptr<Saori> create();

namespace {
    std::unique_ptr<Saori> instance;
}

BOOL load(HGLOBAL *h, long len) {
    instance = create();
    std::string p(reinterpret_cast<char *>(h), len);
    GlobalFree(h);
    instance->load(p);
    return TRUE;
}

BOOL unload() {
    if (instance) {
        instance->unload();
        return TRUE;
    }
    return FALSE;
}

__declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL *h, long *len) {
    if (!instance) {
        GlobalFree(h);
        *len = 0;
        return NULL;
    }
    std::string r(reinterpret_cast<char *>(h), *len);
    GlobalFree(h);
    saori::Request req = saori::Request::parse(r);
    std::string res = instance->request(req);
    *len = res.length();
    HGLOBAL ret = GlobalAlloc(GPTR, res.length());
    memcpy(reinterpret_cast<char *>(ret), res.data(), res.length());
    return ret;
}
