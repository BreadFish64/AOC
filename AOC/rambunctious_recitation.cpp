#include <fstream>
#include <unordered_map>

#include <fmt/ostream.h>

#include "types.hpp"

int main() {
    std::ifstream input_file{"input.txt"};
    std::unordered_map<u32, u32> numbers;
    u32 round{0};
    u32 num{0};
    for (std::istream_iterator<u32> it{input_file}; it != decltype(it){}; ++it) {
        numbers.insert_or_assign(num, round++);
        num = *it;
    }
    const auto AdvanceToRound = [&](u32 stop) {
        for (; round < stop; ++round) {
            auto [it, is_new] = numbers.try_emplace(num, round);
            if (is_new) {
                num = 0;
            } else {
                auto& previous_round = it->second;
                num = round - previous_round;
                previous_round = round;
            }
        }
        fmt::print("Round {}: {}\n", round, num);
    };
    AdvanceToRound(2020);
    AdvanceToRound(30000000);
}
