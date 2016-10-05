/**
    Copyright 2016 Steve Göring

    \author stg7

    \brief helper functions for cppsimhash

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

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>


namespace utils {

    std::string read_file_as_string(std::string filename) {
        std::ifstream infile(filename.c_str());
        std::string result = "";
        std::string line;
        while (std::getline(infile, line)) {
            result += line + "\n";
        }
        return result;
    }

    // split is copied from phrasit/src/utils.hpp
    inline std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.emplace_back(item);
        }
        return elems;
    }

    /*
    *   split a string by delimiter and return result in a vector of strings
    *   \param s string to split
    *   \param delim using this character as a delimiter
    *   \return splitted strings as a vector
    */
    inline std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

    /* ----- new -- */
    void replaceAll(std::string& str, const std::string& from, const std::string& to) {
        if (from.empty())
            return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    std::vector<std::string> tokenize(std::string s) {
        // tidy up string
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        for (auto c : "\n,.-#+*+?=)(/&%$§\"!^<>|;:_\\´`") {
            replaceAll(s, c + std::string(""), " ");
        }

        return split(s, ' ');
    }
}

#endif