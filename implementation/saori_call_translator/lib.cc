// *必ず*ユニークなSAORI名を定義すること。
#define LIBRARY_NAME call_translator

#include <iostream>

#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <thread>

#include "skeleton/skeleton.h"

#include "child_process.h"

// Saoriを継承すること。
class CallTranslator : public Saori {
    private:
        std::mutex mutex_;
        std::unique_ptr<std::thread> th_;
        std::unique_ptr<ChildProcess> process_;
        std::optional<std::string> result_;
        std::thread::id id_;
    public:
        CallTranslator() {}
        ~CallTranslator() {}
        saori::Response request(saori::Request req) override {
            if (!req(0)) {
                saori::Response res {204, "No Content"};
                res["Charset"] = "UTF-8";
                return res;
            }
            std::string command = req(0).value();
            if (command == "call") {
                std::string config_path = req(1).value();
                std::ostringstream oss;
                for (int i = 2; req(i); i++) {
                    oss << req(i).value() << "\n";
                }
                std::string src = oss.str();
                std::unique_ptr<ChildProcess> process = std::make_unique<ChildProcess>();
                if (process->spawn("translator-cli.exe", "--model-config-paths", config_path)) {
                    process->write(src);
                    if (th_) {
                        std::unique_lock<std::mutex> lock(mutex_);
                        id_ = std::this_thread::get_id();
                        result_ = std::nullopt;
                        th_->detach();
                    }
                    th_ = std::make_unique<std::thread>([&](std::unique_ptr<ChildProcess> process) {
                        std::string result = process->read();
                        {
                            std::unique_lock<std::mutex> lock(mutex_);
                            if (std::this_thread::get_id() == id_) {
                                result_ = result;
                            }
                        }
                    }, std::move(process));
                    id_ = th_->get_id();
                    saori::Response res {204, "No Content"};
                    res["Charset"] = "UTF-8";
                    return res;
                }
                else {
                    saori::Response res {200, "OK"};
                    res["Charset"] = "UTF-8";
                    res() = "failed to spawn";
                    return res;
                }
            }
            else if (command == "result") {
                std::optional<std::string> result;
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    if (result_) {
                        result = result_;
                        result_ = std::nullopt;
                    }
                }
                saori::Response res {200, "OK"};
                res["Charset"] = "UTF-8";
                if (result) {
                    th_->join();
                    th_.reset();
                    std::istringstream iss(result.value());
                    std::string line;
                    int index = 0;
                    while (std::getline(iss, line)) {
                        if (line.ends_with("\r")) {
                            line = line.substr(0, line.length() - 1);
                        }
                        if (line.empty()) {
                            continue;
                        }
                        res(index++) = line;
                    }
                    res() = index;
                }
                else {
                    res() = -1;
                }
                return res;
            }
            saori::Response res {204, "No Content"};
            res["Charset"] = "UTF-8";
            return res;
        }
};

// 必須の関数。
// Saoriを継承したclassを返すこと。
std::unique_ptr<Saori> create() {
    return std::make_unique<CallTranslator>();
}
