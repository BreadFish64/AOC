#include "types.hpp"

void SonarSweep(std::span<const int> depths, int window_width) {
    int increased = 0;
    for (int i = window_width; i < depths.size(); ++i)
        if (depths[i - window_width] < depths[i]) ++increased;
    fmt::print("window width {} increases: {}\n", window_width, increased);
}
int main() {
    std::ifstream input_file{"input.txt"};
    const std::vector depths(std::istream_iterator<int>{input_file}, {});
    SonarSweep(depths, 1);
    SonarSweep(depths, 3);
}
