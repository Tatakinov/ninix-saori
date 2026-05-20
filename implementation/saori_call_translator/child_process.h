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
            HANDLE tmp;
            if (!CreatePipe(&tmp, &p2c_[W], nullptr, 0)) {
                return false;
            }
            DuplicateHandle(GetCurrentProcess(), tmp,
                    GetCurrentProcess(), &p2c_[R],
                    0, TRUE, DUPLICATE_SAME_ACCESS);
            CloseHandle(tmp);
            if (!CreatePipe(&c2p_[R], &tmp, nullptr, 0)) {
                CloseHandle(p2c_[R]);
                CloseHandle(p2c_[W]);
                return false;
            }
            DuplicateHandle(GetCurrentProcess(), tmp,
                    GetCurrentProcess(), &c2p_[W],
                    0, TRUE, DUPLICATE_SAME_ACCESS);
            CloseHandle(tmp);
            std::ostringstream oss;
            for (auto &x : args_) {
                oss << x << " ";
            }
            std::string cmd_line = oss.str() + "\0";
            p_ = std::make_optional<process_t>();
            STARTUPINFO si;
            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
            si.wShowWindow = SW_HIDE;
            si.hStdInput = p2c_[R];
            si.hStdOutput = c2p_[W];
            si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
            if (!CreateProcess(nullptr, cmd_line.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &(p_.value()))) {
                CloseHandle(p2c_[R]);
                CloseHandle(p2c_[W]);
                CloseHandle(c2p_[R]);
                CloseHandle(c2p_[W]);
                return false;
            }
            CloseHandle(p2c_[R]);
            CloseHandle(c2p_[W]);
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
                close(p2c_[R]);
                close(c2p_[W]);
                std::vector<char *> argv;
                for (auto &x : args_) {
                    char *tmp = strdup(x.c_str());
                    argv.push_back(tmp);
                }
                argv.push_back(nullptr);
                execvp(argv[0], argv.data());
                // error
                for (char *x : argv) {
                    free(x);
                }
                close(p2c_[W]);
                close(c2p_[R]);
                exit(EXIT_FAILURE);
            }
#endif // OS
            return true;
        }
        void write(std::string data) {
            if (!p_) {
                return;
            }
#if defined(_WIN32) || defined(WIN32)
            DWORD len = 0;
            WriteFile(p2c_[W], data.data(), data.length(), &len, nullptr);
            CloseHandle(p2c_[W]);
#else
            ::write(p2c_[W], data.data(), data.length());
            close(p2c_[W]);
#endif // OS
        }
        std::string read() {
            if (!p_) {
                std::cout << "???" << std::endl;
                return "failed to translate";
            }
            std::ostringstream oss;
            char buffer[1024];
#if defined(_WIN32) || defined(WIN32)
            DWORD len = 0;
            while (true) {
                if (!ReadFile(c2p_[R], buffer, 1024, &len, nullptr) || len <= 0) {
                    break;
                }
                std::string tmp(buffer, len);
                oss << tmp;
            }
            CloseHandle(c2p_[R]);
            WaitForSingleObject(p_.value().hProcess, INFINITE);
            CloseHandle(p_.value().hProcess);
            CloseHandle(p_.value().hThread);
#else
            while (true) {
                ssize_t len = ::read(c2p_[R], buffer, 1024);
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
            return result;
        }
};

#endif // CHILD_PROCESS_H_
