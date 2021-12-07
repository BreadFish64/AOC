#include "types.hpp"

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector crab_starts(std::istream_iterator<int>{input_file}, {});

    // Binary search for the most efficient value
    const int cursor_start =
        std::bit_ceil(static_cast<unsigned>(std::ranges::max(crab_starts))) >> 1;
    const auto MinFuel = [&](auto fuel_func) {
        int cursor = cursor_start;
        for (int delta = cursor_start >> 1; delta != 0; delta >>= 1) {
            int cursor_fuel = fuel_func(cursor);
            if (fuel_func(cursor - 1) < cursor_fuel) {
                cursor -= delta;
            } else if (fuel_func(cursor + 1) < cursor_fuel) {
                cursor += delta;
            } else {
                break;
            }
        }
        return fuel_func(cursor);
    };

    fmt::print("P1: {}\n", MinFuel([&](int pos) {
                   int fuel = 0;
                   for (auto crab : crab_starts) fuel += std::abs(crab - pos);
                   return fuel;
               }));
    fmt::print("P2: {}\n", MinFuel([&](int pos) {
                   int fuel = 0;
                   for (auto crab : crab_starts) {
                       int crab_fuel = std::abs(crab - pos);
                       fuel += crab_fuel * (crab_fuel + 1) / 2;
                   }
                   return fuel;
               }));
}
