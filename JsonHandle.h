#pragma once
#ifndef _JSON_HANDLE
#include <fstream>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#define _JSON_HANDLE
using Json = nlohmann::json;
using std::ofstream;

namespace SaoFU {
    class KeyList {
    public:
        std::pair<vector<bool>, vector<void*>> map;
        int begin = 0;

        template <typename T>
        KeyList& push_back(T val) {
            map.second.push_back(new T(val));
            map.first.push_back(typeid(T).name() == typeid(int).name());
            return *this;
        }

        void clear() {
            begin = map.first.size();
        }

        bool is_integer(int index) {
            return map.first[index];
        }

        template <typename T>
        T get(int index) {
            return *(T*)map.second[index + begin];
        }

        void free() {
            for (int i = 0; i < map.second.size(); i++) {
                if (map.first[i])
                    delete static_cast<int*>(map.second[i]);
                else
                    delete static_cast<const char*>(map.second[i]);
            }
        }
    };

    class JsonHandle {
    public:
        bool is_json;
        Json _json;
        std::string str;
        std::string file_path;
        std::ifstream ifs;
        KeyList key_list;
        vector<Json> json_list;

        JsonHandle(std::string file_path) {
            ifs.open(file_path, std::ios::in);
            this->str = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            this->is_json = (this->_json.accept(str) && ifs.is_open());
            this->file_path = file_path;
            if (this->is_json) {
                this->_json = Json::parse(str);
                json_list.push_back(this->_json);
            }
        }

        JsonHandle(void* enable, std::string str) {
            this->str = str;
            this->is_json = this->_json.accept(str);
            if (this->is_json) {
                this->_json = Json::parse(str);
                json_list.push_back(this->_json);
            }
        }

        JsonHandle(const JsonHandle&) {
        }

        static JsonHandle parse(std::string data) {
            return JsonHandle(nullptr, data);
        }

        template <typename T>
        JsonHandle& operator[](T item) {
            this->_json = this->_json[item];
            this->json_list.push_back(this->_json);
            this->key_list.push_back(item);
            return *this;
        }

        JsonHandle& operator[](string& item) {
            this->_json = this->_json[item];
            this->json_list.push_back(this->_json);
            this->key_list.push_back(item.c_str());
            return *this;
        }

        template <typename T, typename U>
        JsonHandle& updata(T key, U val) {
            Json tmp{{key, val}};
            json_list[json_list.size() - 1].update(tmp);
            this->handle();
            return *this;
        }

        template <typename T>
        JsonHandle& append(T val) {
            json_list[json_list.size() - 1].push_back(val);
            this->handle();
            return *this;
        }

        template <typename T>
        JsonHandle& remove(T val) {
            int len = json_list.size() - 1;
            json_list[len].erase(val);
            this->handle();
            return *this;
        }

        void handle() {
            int len = json_list.size() - 1;
            for (int i = len - 1; i >= 0; i--) {
                if (key_list.is_integer(i))
                    json_list[i][key_list.get<int>(i)] = json_list[i + 1];
                else
                    json_list[i][key_list.get<const char*>(i)] = json_list[i + 1];
            }
            _json = json_list[0];
            json_list.clear();
            key_list.clear();
            json_list.push_back(_json);
            output();
        }

        Json json() {
            Json tmp = _json;
            _json = json_list[0];
            json_list.clear();
            key_list.clear();
            json_list.push_back(_json);
            return tmp;
        }

        void output(string path = "") {
            if (!this->file_path.empty() || !path.empty()) {
                !path.empty() ? void() : void(path = this->file_path);
                ofstream ofs(path);
                ofs << _json.dump(4);
                ofs.close();
            }
        }

        ~JsonHandle() {
            this->key_list.free();
        }
    };
}
#endif
