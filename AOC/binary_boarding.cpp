#include <algorithm>
#include <bitset>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

#include <fmt/ostream.h>

#include "types.hpp"

usize Index(const std::string& str) {
    return std::bitset<7>(str, 0, 7, 'F', 'B').to_ullong() << 3 |
           std::bitset<3>(str, 7, 3, 'L', 'R').to_ullong();
}

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<usize> indices{};
    std::transform(std::istream_iterator<std::string>{input_file}, {}, std::back_inserter(indices),
                   Index);
    std::ranges::sort(indices);
    fmt::print("highest seat index: {}\n", indices.back());
    fmt::print("index of my seat: {}",
               *std::ranges::adjacent_find(indices, [](auto x, auto y) { return y != x + 1; }) + 1);
}
