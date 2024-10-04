#ifndef SSTP_HEADER_H_
#define SSTP_HEADER_H_

#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>

#include "base/optional.h"

namespace base {

class Header {
    public:
        Header();
        ~Header();
        static Header parse(std::string str);
        static Header parse(std::istringstream& iss);
        void remove(const std::string& key);
        inline optional& operator[](const std::string& key) { return map_[key]; }
        operator std::string() const {
            std::ostringstream oss;
            auto h = map_;
            // Charsetは他のヘッダより優先する
            if (h["Charset"]) {
                oss << "Charset: " << h["Charset"].value();
                h.erase("Charset");
            }
            for (auto& [k, v] : h) {
                if (v) {
                    oss << k << ": " << v.value() << "\x0d\x0a";
                }
            }
            return oss.str();
        }
    private:
        std::unordered_map<std::string, optional> map_;
};

}

#endif // SSTP_HEADER_H_
