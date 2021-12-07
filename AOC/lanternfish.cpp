#include "types.hpp"

int main() {
    std::ifstream input_file{"input.txt"};
    std::array<u64, 7> fish{};
    std::array<u64, 7> new_fish{};
    std::for_each(std::istream_iterator<unsigned>{input_file}, {},
                  [&](unsigned day) { ++fish[day]; });
    const auto SimulateFish = [&](unsigned begin, unsigned end) {
        for (unsigned day = begin; day < end; ++day) {
            u64& day_cell = fish[day % 7];
            new_fish[(day + 2) % 7] = day_cell;
            day_cell += std::exchange(new_fish[day % 7], 0);
        }
        u64 fish_count = std::reduce(fish.begin(), fish.end());
        fish_count += std::reduce(new_fish.begin(), new_fish.end());
        fmt::print("{} days {}\n", end, fish_count);
    };
    SimulateFish(0, 80);
    SimulateFish(80, 256);
}
