#include "types.hpp"

struct Basin {
    ssize y, x;
    u64 size;
};

std::basic_ifstream<u8> input_file{"input.txt"};
std::vector input(std::istreambuf_iterator<u8>{input_file}, {});
const ssize width = std::ranges::find(input, '\n') - input.begin();
const ssize stride = width + 1;
const ssize height = input.size() / stride;
u8* Row(ssize row) { return input.data() + stride + stride * row; }

std::vector<bool> searched(stride*(height + 2));
auto IsSearched(ssize y, ssize x) { return searched[stride + stride * y + x]; }
u64 Cancer(ssize y, ssize x) {
    IsSearched(y, x) = true;
    unsigned point = Row(y)[x];
    if (point == '9') return 0;
    usize size = 1;
    if (!IsSearched(y, x - 1) && Row(y)[x - 1] >= point) size += Cancer(y, x - 1);
    if (!IsSearched(y, x + 1) && Row(y)[x + 1] >= point) size += Cancer(y, x + 1);
    if (!IsSearched(y - 1, x) && Row(y - 1)[x] >= point) size += Cancer(y - 1, x);
    if (!IsSearched(y + 1, x) && Row(y + 1)[x] >= point) size += Cancer(y + 1, x);
    return size;
}

int main() {
    input.insert(input.begin(), stride, '9');
    input.insert(input.end(), stride, '9');
    for (ssize y = -1; y <= height; ++y) Row(y)[width] = '9';

    u64 risk_sum = 0;
    std::vector<Basin> low_points;
    for (ssize y = 0; y < height; ++y) {
        for (ssize x = 0; x < width; ++x) {
            unsigned point = Row(y)[x];
            unsigned adjacent = Row(y)[x - 1];
            if (point >= adjacent) continue;
            adjacent = Row(y)[x + 1];
            if (point >= adjacent) continue;
            adjacent = Row(y - 1)[x];
            if (point >= adjacent) continue;
            adjacent = Row(y + 1)[x];
            if (point >= adjacent) continue;
            risk_sum += point - '0' + 1;
            low_points.emplace_back(Basin{y, x, 0});
        }
    }
    fmt::print("{}\n", risk_sum);

    for (auto& [y, x, size] : low_points) {
        for (usize i = 0; i < searched.size(); ++i) searched[i] = false;
        size = Cancer(y, x);
    }
    std::ranges::sort(low_points, std::greater{}, [](auto basin) { return basin.size; });
    u64 greatest_basin_product = low_points[0].size * low_points[1].size * low_points[2].size;
    fmt::print("{}\n", greatest_basin_product);
}
