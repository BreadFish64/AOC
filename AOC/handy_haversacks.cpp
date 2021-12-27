#include <bitset>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "types.hpp"

int main() {
    std::ifstream input_file{"input.txt"};

    std::unordered_map<std::string, std::pair<std::string, usize>> color_map;
    std::string str;
    while (std::getline(input_file, str, " bags contain ")) {
    }
}