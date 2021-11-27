#include <corecrt_io.h>
#include "core.h"

#include <fstream>
#include <iomanip>
#include <sstream>

using std::vector;
using std::cout;
using std::string;

vector<string> SaoFU::list_dir(string path, string ext) {
    struct _finddata_t fileinfo;
    vector<string> file;
    long long int handle = _findfirst((path + "\\" + ext).c_str(), &fileinfo);
    do {
        file.emplace_back(path + "\\" + fileinfo.name);
    } while (!_findnext(handle, &fileinfo));
    _findclose(handle);
    return file;
}

vector<string> SaoFU::string_split(string split_str, string delimiter) {
    //分割特定字元
    int pos_start = 0, pos_end, delim_len = delimiter.length();
    vector<string> res;

    while ((pos_end = split_str.find(delimiter, pos_start)) != string::npos) {
        string token = split_str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(split_str.substr(pos_start));
    return res;
}

std::string SaoFU::string_split(string split_str, string left, string right) {
    string spilit = SaoFU::string_split(split_str, right)[1];
    int end = spilit.find(left, 0);
    return spilit.substr(0, end);
}

string SaoFU::url_encode(const string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char)c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}

string SaoFU::get_time(string s) {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    if (s == "now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if (s == "date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return string(buf);
}

std::string SaoFU::param_fixed(int num) {
    //參數必須符合指定數目 x == num
    return std::to_string(num);
}

std::string SaoFU::param_variadic(int num) {
    //參數不得小於等於指定數目，指定數目之後為不定引數 x <= num
    return std::to_string(-num);
}

std::string SaoFU::param_unspecified(int num) {
    //(預設)參數不得小於指定數目，指定數目(含)之後為不定引數 x < num
    return std::to_string(-num * 1000);
}

int SaoFU::rand_generate(int min, int max) {
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_int_distribution<int> d(min, max);
    return d(engine);
}

int SaoFU::rand_generate(int num) {
    if (num > 0) {
        return rand_generate(0, num);
    }
    return 0;
}
