#ifndef HEADER_H_
#define HEADER_H_

#include "plugin.h"
#include "saori.h"
#include "shiori.h"
#include "sstp.h"

template<class T, class U>
class Library {
    public:
        Library() {}
        virtual ~Library() {}
        virtual bool load(std::string path) { return true; }
        virtual U request(T req) {
            U res {204, "No Content"};
            return res;
        }
        virtual bool unload() { return true; }
};

using Saori = Library<saori::Request, saori::Response>;
using Plugin = Library<plugin::Request, plugin::Response>;

#endif // HEADER_H_
