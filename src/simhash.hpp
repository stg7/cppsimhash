/**
    Copyright 2016 Steve Göring

    \author stg7

    \brief cppsimhash functions

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

#ifndef SIMHASH_H
#define SIMHASH_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>

#include "haesni.hpp"
#include "utils.hpp"


namespace hash {

    unsigned long simhash(const std::string s) {
        // TODO(stg7): maybe add ngram (2..3) creation for extending bag of word approach
        std::map<std::string, long> features;
        auto tokens = utils::tokenize(s);
        // build bag of word representation
        for (auto& t: tokens) {
            if (t == "") {  ///< ignore empty tokens
                continue;
            }
            if (features.find(t) == features.end()) {
                features[t] = 0;
            }
            features[t] = features[t] + 1;
        }

        // build 2 grams
        for (unsigned long i = 1; i < tokens.size(); i ++) {
            auto t1 = tokens[i - 1];
            auto t2 = tokens[i];
            if (t1 == "") {  ///< ignore empty tokens
                continue;
            }

            auto _2gram = t1 + " " + t2;
            if (features.find(_2gram) == features.end()) {
                features[_2gram] = 0;
            }
            features[_2gram] = features[_2gram] + 2;
        }

        // build 3 grams
        for (unsigned long i = 2; i < tokens.size(); i ++) {
            auto t1 = tokens[i - 2];
            auto t2 = tokens[i - 1];
            auto t3 = tokens[i];
            if (t1 == "") {  ///< ignore empty tokens
                continue;
            }

            auto _3gram = t1 + " " + t2 + " " + t3;
            if (features.find(_3gram) == features.end()) {
                features[_3gram] = 0;
            }
            features[_3gram] = features[_3gram] + 3;
        }
        // do simhashing
        std::vector<long> sim_vec(64);
        for (auto x: features) {
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

    double similarity(const std::string s, const std::string k) {
        unsigned long hash_s = simhash(s);
        unsigned long hash_k = simhash(k);
        return 1 - hamming_distance(hash_s, hash_k) / 64.0;
    }
}

#endif
