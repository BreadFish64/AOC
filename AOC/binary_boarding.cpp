#include <algorithm>
#include <bitset>
#include <fstream>
#include <numeric>
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
    auto [min, max] = std::ranges::minmax_element(indices);
    fmt::print("highest seat index: {}\n", *max);
    usize n = indices.size();
    fmt::print("index of my seat: {}",
               (n + *min * 2) * (n + 1) / 2 - std::reduce(indices.begin(), indices.end()));
}
