
#include <string>
#include <iostream>
#include <experimental/filesystem>

#include "simhash.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
    namespace fs = std::experimental::filesystem;
    // TODO: better argument handling or seperate files for doing simhash
    // and distance calculation
    if (argc == 1) {
        std::string content = "";
        std::string line;
        while (std::getline(std::cin, line)) {
            content = content + " " + line;
        }
        std::cout << hash::simhash(content) << std::endl;
    }

    if (argc == 2) {
        std::cout << hash::simhash(utils::read_file_as_string(argv[1])) << std::endl;
    }

    if (argc == 3) {
        if (!fs::exists(argv[1]) || !fs::exists(argv[2])) {
            std::cout << "[ERROR] there is something wrong with your input files "
                << argv[1] << " " << argv[2] << std::endl;
            return 1;
        }
        std::cout << "calculate similarity between: " << argv[1] << " and " << argv[2] << std::endl;
        auto sim = hash::similarity(utils::read_file_as_string(argv[1]), utils::read_file_as_string(argv[2]));

        std::cout << "= " << sim << std::endl;
    }
    return 0;
}