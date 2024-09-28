#define LIBRARY_NAME test

#include "skeleton/unix/lib.cc"

class Test : public Saori {
    public:
        std::string request(saori::Request req) override {
            saori::Response res {200, "OK"};
            res() = "test";
            return res;
        }
};

std::shared_ptr<Saori> create() {
    return std::make_shared<Test>();
}
