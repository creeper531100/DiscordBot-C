#pragma once
#include <functional>

#include "request_data.h"

#ifndef _CORE

#include <random>
#include <string>
#include <vector>
#include <time.h>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#define _CORE

namespace SaoFU {
    std::vector<std::string> list_dir(std::string, std::string ext = "*.*");
    std::vector<std::string> string_split(std::string, std::string);
    std::string string_split(std::string, std::string, std::string);
    std::string url_encode(const std::string&);
    std::string get_time(std::string);
    int rand_generate(int num);
    int rand_generate(int min, int max);

    template <typename T, typename U>
    void transform_item(T t, U& u) {
        for (auto it = t.begin(); it != t.end(); ++it) {
            u.push_back(it.key());
        }
    }

    std::string param_fixed(int);
    std::string param_variadic(int);
    std::string param_unspecified(int);

    template <typename T, typename = std::enable_if_t<!std::is_integral<T>::value>>
    auto rand_generate(T type) {
        if (type.size() > 0) {
            std::random_device rd{};
            std::mt19937 engine{rd()};
            std::uniform_int_distribution<int> d(0, type.size() - 1);
            return type[d(engine)];
        }
        return type[0];
    }

    template <typename Iterable>
    class enumerate_object {
    private:
        Iterable _iter;
        std::size_t _size;
        decltype(std::begin(_iter)) _begin;
        const decltype(std::end(_iter)) _end;
    public:
        enumerate_object(Iterable iter) :
            _iter(iter),
            _size(0),
            _begin(std::begin(iter)),
            _end(std::end(iter)) {
        }

        const enumerate_object& begin() const { return *this; }
        const enumerate_object& end() const { return *this; }

        bool operator!=(const enumerate_object&) const {
            return _begin != _end;
        }

        void operator++() {
            ++_begin;
            ++_size;
        }

        std::pair<std::size_t, decltype(*_begin)> operator*() const {
            return {_size, *_begin};
        }
    };

    template <typename Iterable>
    enumerate_object<Iterable> enumerate(Iterable&& iter) {
        return {std::forward<Iterable>(iter)};
    }


    template <typename T, bool is_str = std::is_same<T, const char*>::value || std::is_same<T, std::string>::value>
    void Logger(T write_data) {
        std::string filePath = "DATA\\log\\" + get_time("date") + ".txt";
        std::string now = get_time("now");
        std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
        ofs << now << '\n';
        if constexpr (is_str) {
            ofs << write_data << '\n';
        }
        else {
            for (auto& row : enumerate(write_data)) {
                ofs << "Data: " << row.first << '\n' << row.second << '\n';
            }
        }
        ofs.close();
    }

    enum FIND_TYPE {
        SEARCH_CONTENT = 0,
        DEFAULT = 1
    };

    template <typename T, FIND_TYPE enable = DEFAULT>
    class BaseFindObject {
    public:
        int index;
        bool is_true;
        T obj;
        void* base_data;

        BaseFindObject(T obj) : index(-1), is_true(false), obj(obj) {
        }

        ~BaseFindObject() {}

        template <typename U>
        std::enable_if_t<(std::is_same<U, std::string>::value && enable), BaseFindObject&>
        in(U v) {
            index = v.find(obj);
            if (index != std::string::npos) {
                is_true = true;
                auto element = v[index];
                base_data = &element;
            }
            return *this;
        }

        template <typename U>
        std::enable_if_t<(!std::is_same<U, std::string>::value) && enable, BaseFindObject&>
        in(U v) {
            auto it = std::find(v.begin(), v.end(), obj);
            if (it != v.end()) {
                index = std::distance(v.begin(), it);
                auto element = v[index];
                base_data = &element;
                is_true = true;
            }
            return *this;
        }

        template <typename U, bool sw = enable>
        std::enable_if_t<!sw, BaseFindObject&>
        in(U v) {
            for (auto& row : enumerate(v)) {
                if (row.second.find(obj) != string::npos) {
                    index = row.first;
                    auto element = row.second;
                    base_data = &element;
                    is_true = true;
                }
            }
            return *this;
        }

        template <typename U>
        U data() {
            return *static_cast<U*>(base_data);
        }
    };
    //enable
    template <FIND_TYPE enable = DEFAULT, typename T>
    BaseFindObject<T, enable> find(T t) {
        return BaseFindObject<T, enable>(t);
    }
}
#endif
