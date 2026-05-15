#ifndef CHILD_PROCESS_H_
#define CHILD_PROCESS_H_

#include <iostream>

#include <optional>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
using fd_t = HANDLE;
using process_t = PROCESS_INFORMATION;
#else
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using fd_t = int;
using process_t = pid_t;
#endif // OS

namespace {
    const int R = 0;
    const int W = 1;
}

class ChildProcess {
    private:
        std::vector<std::string> args_;
        fd_t p2c_[2], c2p_[2];
        std::optional<process_t> p_;
    public:
        ChildProcess() {}
        ~ChildProcess() {}
        template<typename Head, typename... Remain>
        bool spawn(Head&& h, Remain&&... remain) {
            args_.push_back(h);
            return spawn(std::forward<Remain>(remain)...);
        }
        bool spawn() {
#if defined(_WIN32) || defined(WIN32)
#else
            if (pipe(p2c_)) {
                return false;
            }
            if (pipe(c2p_)) {
                close(p2c_[R]);
                close(p2c_[W]);
                return false;
            }
            // error
            if ((p_ = fork()).value() == -1) {
                close(p2c_[R]);
                close(p2c_[W]);
                close(c2p_[R]);
                close(c2p_[W]);
                return false;
            }
            // parent
            else if (p_.value()) {
                close(p2c_[R]);
                close(c2p_[W]);
                return true;
            }
            // child
            else {
                close(p2c_[W]);
                close(c2p_[R]);
                if (dup2(p2c_[R], R) == -1) {
                    close(p2c_[R]);
                    close(p2c_[W]);
                    close(c2p_[R]);
                    close(c2p_[W]);
                    exit(EXIT_FAILURE);
                }
                if (dup2(c2p_[W], W) == -1) {
                    close(p2c_[R]);
                    close(p2c_[W]);
                    close(c2p_[R]);
                    close(c2p_[W]);
                    exit(EXIT_FAILURE);
                }
                std::vector<char *> argv;
                for (auto &x : args_) {
                    char *tmp = strdup(x.c_str());
                    argv.push_back(tmp);
                }
                argv.push_back(nullptr);
                execvp(argv[0], argv.data());
                // error
                exit(EXIT_FAILURE);
            }
#endif // OS
        }
        void write(std::string data) {
            if (!p_) {
                return;
            }
#if defined(_WIN32) || defined(WIN32)
#else
            std::cout << "write: " << ::write(p2c_[W], data.data(), data.length()) << " " << data << std::endl;
            close(p2c_[W]);
#endif // OS
        }
        std::string read() {
            if (!p_) {
                std::cout << "???" << std::endl;
                return "";
            }
            std::ostringstream oss(std::ios::binary);
            char buffer[1024];
#if defined(_WIN32) || defined(WIN32)
#else
            while (true) {
                ssize_t len = ::read(c2p_[R], buffer, 1024);
                std::cout << "len:" << len << std::endl;
                if (len <= 0) {
                    break;
                }
                std::string tmp(buffer, len);
                oss << tmp;
            }
            close(c2p_[R]);
            int wstatus;
            waitpid(p_.value(), &wstatus, 0);
#endif
            p_ = std::nullopt;
            std::string result = oss.str();
            std::cout << "rrresult" << std::endl;
            std::cout << result << std::endl;
            return result;
        }
};

#endif // CHILD_PROCESS_H_
