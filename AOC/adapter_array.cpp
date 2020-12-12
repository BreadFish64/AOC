#include <algorithm>
#include <fstream>
#include <ranges>
#include <thread>
#include <vector>

#include <fmt/ostream.h>

#include "types.hpp"

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<unsigned> adapters{std::istream_iterator<unsigned>{input_file}, {}};
    std::ranges::sort(adapters);
    adapters.emplace_back(adapters.back() + 3);
    {
        std::array<unsigned, 4> differences{};
        {
            unsigned previous = 0;
            for (unsigned adapter : adapters) {
                ++differences[adapter - previous];
                previous = adapter;
            }
        }
        fmt::print("Distribution:\n1: {}\n2: {}\n3: {}\n", differences[1], differences[2],
                   differences[3]);
        fmt::print("1x3: {}\n", differences[1] * differences[3]);
    }
    adapters.insert(adapters.begin(), 0);
    {
        // dummy value to avoid adding more conditionals
        adapters.emplace_back(adapters.back() + 1);
        std::vector<u64> combos(adapters.size() - 1);
        combos[combos.size() - 1] = 1;
        for (s64 idx = combos.size() - 2; idx >= 0; --idx)
            for (s64 next = idx + 1; adapters[next] - adapters[idx] <= 3; ++next)
                combos[idx] += combos[next];
        adapters.pop_back();
        fmt::print("Combinations: {}\n", combos[0]);
    }
}
