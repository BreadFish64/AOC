#include "types.hpp"

int main() {
    std::ifstream input_file{"input.txt"};
    const std::vector depths(std::istream_iterator<int>{input_file}, {});
    const auto Solve = [&](int window_width) {
        int increased = 0;
        for (int i = 0; i < depths.size() - window_width; ++i)
            if (depths[i] < depths[i + window_width]) ++increased;
        fmt::print("window width {} increases: {}\n", window_width, increased);
    };
    Solve(1);
    Solve(3);
}
