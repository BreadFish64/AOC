#include <fstream>
#include <ranges>
#include <vector>

#include <fmt/ostream.h>

#include "types.hpp"

static std::optional<std::tuple<u32, u32>> ReportRepairOne(std::span<const u32> data,
                                                           u32 target) noexcept {
    if (data.size() < 2) return std::nullopt;
    auto front = data.begin();
    auto rear = data.end() - 1;
    while (front != rear) {
        u32 sum = *front + *rear;
        if (sum < target) {
            ++front;
        } else if (sum > target) {
            --rear;
        } else {
            break;
        }
    }
    return (*front + *rear == target) ? std::make_optional(std::make_pair(*front, *rear))
                                      : std::nullopt;
}

static std::optional<std::tuple<u32, u32, u32>> ReportRepairTwo(std::span<const u32> data,
                                                                u32 target) noexcept {
    if (data.size() < 3) return std::nullopt;
    auto moving_target = data.end();
    auto moving_rear = data.begin();
    std::optional<std::tuple<u32, u32>> solution;
    while (!solution) {
        --moving_target;
        if (moving_target <= moving_rear) return std::nullopt;
        u32 temp_target = 2020 - *moving_target;
        if (auto temp_rear = std::lower_bound(moving_rear, moving_target, temp_target);
            temp_rear == data.end()) {
            continue;
        } else {
            moving_rear = temp_rear;
        }
        solution = ReportRepairOne(std::span{data.begin(), moving_rear}, temp_target);
    }
    return std::make_optional(std::tuple_cat(*solution, std::make_tuple(*moving_target)));
}

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<u32> dataset{std::istream_iterator<u32>{input_file}, {}};
    std::ranges::sort(dataset);
    {
        auto solution = ReportRepairOne(dataset, 2020);
        if (solution) {
            auto [a, b] = *solution;
            fmt::print("Solution for two numbers: {}\n", a * b);
        } else {
            fmt::print("Failed to find a solution for two numbers\n");
        }
    }
    {
        auto solution = ReportRepairTwo(dataset, 2020);
        if (solution) {
            auto [a, b, c] = *solution;
            fmt::print("Solution for three numbers: {}\n", a * b * c);
        } else {
            fmt::print("Failed to find a solution for three numbers\n");
        }
    }
}