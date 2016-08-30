
#ifndef SIMHASH_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>

#include "haesni.hpp"
#include "utils.hpp"


namespace hash {

    unsigned long simhash(const std::string s) {
        // build bag of word representation
        // TODO(stg7): maybe add ngram (2..3) creation for extending bag of word approach
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

    double similarity(const std::string s, const std::string k) {
        unsigned long hash_s = simhash(s);
        unsigned long hash_k = simhash(k);
        return 1 - hamming_distance(hash_s, hash_k) / 64.0;
    }
}

#endif
