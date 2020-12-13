#include <fstream>
#include <numeric>

#include <fmt/ostream.h>

#include "types.hpp"

template <std::integral I>
I AlignUp(I num, I alignment) {
    auto mod = num % alignment;
    return mod == 0 ? num : (num + alignment - mod);
}

int main() {
    std::ifstream input_file{"input.txt"};
    u64 earliest_departure{};
    input_file >> earliest_departure;
    std::vector<std::pair<u64, u64>> ids;
    {
        std::string id;
        u64 position{0};
        while (std::getline(input_file, id, ',')) {
            if (id != "x") {
                auto num = std::stoul(id);
                ids.emplace_back(num, position);
            }
            ++position;
        }
    }
    {
        auto min = std::ranges::min_element(ids, [earliest_departure](const auto& lhs,
                                                                      const auto& rhs) {
            return AlignUp(earliest_departure, lhs.first) < AlignUp(earliest_departure, rhs.first);
        });
        auto id = min->first;
        const u64 departure = AlignUp(earliest_departure, id);
        fmt::print("Id: {}\nDeparture: {}\nAnswer: {}\n\n", id, departure,
                   (departure - earliest_departure) * id);
    }
    {
        // Check increments of the least common multiple of all the previous numbers to find spots
        // where the number is in the right place relative to the others
        u64 lcm = ids.cbegin()->first;
        u64 start = 0;
        for (auto [id, position] : ids) {
            while ((start + position) % id != 0) start += lcm;
            lcm = std::lcm(lcm, id);
        }
        fmt::print("Answer 2: {}\n", start);
    }
}