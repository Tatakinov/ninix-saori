// *必ず*ユニークなSAORI名を定義する
#define LIBRARY_NAME example

// includeするのは.hじゃなくて.cc。
#include "skeleton/unix/saori.cc"

class Example : public Saori {
    public:
        saori::Response request(saori::Request req) override {
            if (!req(0)) {
                saori::Response res {204, "No Content"};
                return res;
            }
            saori::Response res {200, "OK"};
            res() = "This is example.";
            res(0) = req(0).value();
            return res;
        }
};

std::unique_ptr<Saori> create() {
    return std::make_unique<Example>();
}
