// *必ず*ユニークなSAORI名を定義すること。
#define LIBRARY_NAME example

// includeするのは.hじゃなくて.cc。
#include "skeleton/unix/saori.cc"

// Saoriを継承すること。
class Example : public Saori {
    public:
        saori::Response request(saori::Request req) override {
            // ArgumentNがあるかどうかを調べるにはreq(N)とする。
            // req["ArgumentN"]でも良い。
            if (!req(0)) {
                saori::Response res {204, "No Content"};
                return res;
            }
            saori::Response res {200, "OK"};
            // Resultはres()でアクセス出来る。
            // res["Result"]でも良い。
            res() = "This is example.";
            // 値を取り出す時は.value()とする。
            std::string v = req(0).value();
            // ValueNはres(N)でアクセス出来る。
            // res["ValueN"]でも良い。
            res(0) = v;
            // Result,ValueN以外は[]でアクセスする。
            res["Charset"] = "UTF-8";
            return res;
        }
};

// 必須の関数。
// Saoriを継承したclassを返すこと。
std::unique_ptr<Saori> create() {
    return std::make_unique<Example>();
}
