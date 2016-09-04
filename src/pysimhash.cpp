
#include <iostream>
#include <string>
#include <cstring>

#include "simhash.hpp"
#include "utils.hpp"

extern "C" {
    unsigned long simhash_file(wchar_t* s) {
        std::wstring ws(s);
        std::string filename(ws.begin(), ws.end());
        return hash::simhash(utils::read_file_as_string(filename));
    }
    unsigned long simhash(wchar_t* s) {
        std::wstring ws(s);
        std::string _s(ws.begin(), ws.end());
        return hash::simhash(_s);
    }

    int hamming_distance(unsigned long x, unsigned long y) {
        return hash::hamming_distance(x, y);
    }

    void bla() {
        std::cout << "bla" << std::endl;
    }
}