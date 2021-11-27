#pragma once

#ifndef _REQUEST_DATA_

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define CURL_STATICLIB
#include "sleepy_discord/sleepy_discord.h"
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")
#define _REQUEST_DATA_

using std::vector;
using std::cout;
using std::string;

inline static size_t my_write(void* buf, size_t size, size_t nmemb, void* param) {
    string& text = *static_cast<string*>(param);
    size_t totalsize = size * nmemb;
    text.append(static_cast<char*>(buf), totalsize);
    return totalsize;
}

namespace SaoFU {
    class UrlHandle {
    public:
        string url;
        string data;
        int err_code = 0;
        UrlHandle(string url) {
            this->url = url;
        }

        UrlHandle& urlopen() {
            CURL* curl;
            CURLcode res;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                res = curl_easy_perform(curl);

                if (res != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                    err_code = res;
                }

                curl_easy_cleanup(curl);
                curl_global_cleanup();
            }
            return *this;
        }

        UrlHandle& urlpost(string json_data) {
            CURL* curl;
            CURLcode res;
            curl_slist* headers = NULL;

            headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
            string content_length = "Content-Length: " + std::to_string(json_data.size());
            headers = curl_slist_append(headers, content_length.c_str());
            headers = curl_slist_append(headers, "Connection: close");
            headers = curl_slist_append(headers, "dataType: json");

            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();

            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

                res = curl_easy_perform(curl);

                if (res != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                    err_code = res;
                }
                curl_easy_cleanup(curl);
                curl_global_cleanup();
            }
            return *this;
        }

        vector<string> search(string label) {
            std::stringstream ss(data);
            string to;
            vector<string> result;
            while (getline(ss, to, '\n')) {
                if (to.find(label) != string::npos) {
                    result.emplace_back(to);
                }
            }
            if (result.empty())
                result.emplace_back("<None>");
            return result;
        }
    };

}
#endif
