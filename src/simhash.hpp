
#ifndef SIMHASH_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <bitset>

#include "haesni.hpp"

namespace utils {
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

namespace hash {

    unsigned long simhash(const std::string s) {
        // build bag of word representation
        std::map<std::string, long> bag_of_words;
        auto tokens = utils::tokenize(s);
        for (auto& t: tokens) {
            if (t == "") {  ///< ignore empty tokens
                continue;
            }
            if (bag_of_words.find(t) == bag_of_words.end()) {
                bag_of_words[t] = 0;
            }
            bag_of_words[t] = bag_of_words[t] + 1;
        }

        // do simhashing
        std::vector<long> sim_vec(64);
        for (auto x: bag_of_words) {
            auto token = x.first;
            auto freq = x.second;
            unsigned long hash = haesni::hash(&token[0], token.length());
            auto bitset = std::bitset<64>(hash);
            for(int i = 0; i < 64; i++) {
                if (bitset[i] == 1) {
                    sim_vec[i] += freq;
                } else {
                    sim_vec[i] -= freq;
                }
            }
        }

        // build result value
        auto res = std::bitset<64>();
        for (int i = 0; i < 64; i++) {
            if (sim_vec[i] < 0) {
                res[i] = 0;
            } else {
                res[i] = 1;
            }
        }
        return res.to_ulong();
    }

    int hamming_distance(unsigned long x, unsigned long y) {
        int dist = 0;
        unsigned long val = x^y;

        while (val != 0) {
            dist++;
            val &= val -1;
        }
        return dist;
    }

    int distance(const std::string s, const std::string k) {
        unsigned long hash_s = simhash(s);
        unsigned long hash_k = simhash(k);
        return hamming_distance(hash_s, hash_k);
    }
}

#endif