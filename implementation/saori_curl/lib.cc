#define LIBRARY_NAME saori_curl

#include "skeleton/unix/saori.cc"

#include <algorithm>
#include <curl/curl.h>

size_t write(char *buf, size_t size, size_t nmemb, void *userdata);

class Curl : public Saori {
    private:
        CURLcode ret_;
        CURL *hnd_;
        std::string buffer_;

    public:
        saori::Response request(saori::Request req) override {
            if (!req(0)) {
                saori::Response res {204, "No Content"};
                return res;
            }
            buffer_.clear();
            hnd_ = curl_easy_init();
            curl_easy_setopt(hnd_, CURLOPT_BUFFERSIZE, 102400L);
            curl_easy_setopt(hnd_, CURLOPT_URL, req(0).value().c_str());
            curl_easy_setopt(hnd_, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(hnd_, CURLOPT_USERAGENT, "saori-curl/7.88.1");
            curl_easy_setopt(hnd_, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(hnd_, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(hnd_, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
            curl_easy_setopt(hnd_, CURLOPT_FTP_SKIP_PASV_IP, 1L);
            curl_easy_setopt(hnd_, CURLOPT_TCP_KEEPALIVE, 1L);
            curl_easy_setopt(hnd_, CURLOPT_WRITEFUNCTION, &write);
            curl_easy_setopt(hnd_, CURLOPT_WRITEDATA, this);
            curl_easy_setopt(hnd_, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);

            ret_ = curl_easy_perform(hnd_);

            curl_easy_cleanup(hnd_);
            hnd_ = NULL;

            auto it = std::remove_if(buffer_.begin(), buffer_.end(), [](char c) {
                return c == '\r' || c == '\n';
            });
            buffer_.erase(it, buffer_.end());

            saori::Response res {200, "OK"};
            res["Charset"] = "UTF-8";
            res() = buffer_;
            if (ret_ != CURLE_OK) {
                res(0) = curl_easy_strerror(ret_);
            }
            buffer_.clear();
            return res;
        }

        void append(std::string& s) {
            buffer_ += s;
        }
};

size_t write(char *buf, size_t size, size_t nmemb, void *userdata) {
    Curl *p = static_cast<Curl *>(userdata);
    std::string b(buf, size * nmemb);
    p->append(b);
    return size * nmemb;
}

std::unique_ptr<Saori> create() {
    return std::make_unique<Curl>();
}
