
#include <string>
#include <iostream>
#include <fstream>

#include "simhash.hpp"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::string content = "";
        std::string line;
        while (std::getline(std::cin, line)) {
            content = content + " " + line;
        }

        auto x = hash::simhash(content);
        std::cout << x << std::endl;
    }

    if (argc == 3) {
        std::cout << "calculate distance between: " << argv[1] << " and " << argv[2] << std::endl;
        // TODO: check if it is a file
        std::ifstream infile(argv[1]);

    }
    return 0;
}