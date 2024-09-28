#ifndef HEADER_H_
#define HEADER_H_

#include <memory>

#include "plugin.h"
#include "saori.h"
#include "shiori.h"
#include "sstp.h"

class Saori {
    public:
        Saori() {}
        virtual ~Saori() {}
        virtual bool load(std::string path) { return true; }
        virtual std::string request(saori::Request req) {
            saori::Response res {204, "No Content"};
            return res;
        }
        virtual bool unload() { return true; }
};

std::shared_ptr<Saori> create();

#endif // HEADER_H_
