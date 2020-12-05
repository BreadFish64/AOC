#include <algorithm>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

#include <fmt/ostream.h>

#include "types.hpp"

usize Index(const std::string& string) {
    usize idx{0};
    for (char c : std::string_view{string.data(), 7}) {
        idx <<= 1;
        idx |= (c == 'B');
    }
    for (char c : std::string_view{string.data() + 7, 3}) {
        idx <<= 1;
        idx |= (c == 'R');
    }
    return idx;
}

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<usize> indices{};
    std::transform(std::istream_iterator<std::string>{input_file}, {}, std::back_inserter(indices),
                   Index);
    fmt::print("highest seat index: {}\n", *std::ranges::max_element(indices));
    std::array<char, 1024> plane{};
    std::ranges::fill(plane, 'O');
    for (usize seat : indices) plane[seat] = 'X';
    auto search_start = std::find(plane.begin(), plane.end(), 'X');
    auto my_seat = std::find(search_start, plane.end(), 'O');
    fmt::print("index of my seat: {}", my_seat - plane.begin());
}
