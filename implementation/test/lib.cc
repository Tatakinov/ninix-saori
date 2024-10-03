// *必ず*ユニークなSAORI名を定義する
#define LIBRARY_NAME test

// includeするのは.hじゃなくて.cc。
#include "skeleton/unix/saori.cc"

class Test : public Saori {
    public:
        saori::Response request(saori::Request req) override {
            saori::Response res {200, "OK"};
            res() = "test";
            return res;
        }
};

std::unique_ptr<Saori> create() {
    return std::make_unique<Test>();
}
