/**
    Copyright 2016 Steve Göring

    \author stg7

    \brief shared lib for python wrapper

**/
/**
    This file is part of cppsimhash.

    cppsimhash is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cppsimhash is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cppsimhash.  If not, see <http://www.gnu.org/licenses/>.
**/

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