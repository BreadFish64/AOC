#include <fstream>
#include <string>

#include <fmt/ostream.h>

#include "types.hpp"

struct Map {
    u32 width;
    u32 height;
    std::string data;

    friend std::istream& operator>>(std::istream& stream, Map& map) {
        std::getline(stream, map.data);
        map.width = map.data.size();
        map.data.insert(map.data.end(), std::istream_iterator<char>{stream}, {});
        map.height = map.data.size() / map.width;
        return stream;
    }

    std::span<const char> Row(usize row) const {
        return std::span{data.data() + row * width, width};
    }

public:
    u32 TreesInSlope(u32 rise, u32 run) const {
        u32 tree_count{0};
        for (u32 x{run}, y{rise}; y < height; x += run, y += rise) {
            // assumes run is less than width...
            if (x >= width) x -= width;
            if (Row(y)[x] == '#') ++tree_count;
        }
        return tree_count;
    }
};

int main() {
    std::ifstream input_file{"input.txt"};
    Map map;
    input_file >> map;
    std::initializer_list<std::pair<u32, u32>> slopes{{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
    u64 product{1};
    for (auto [run, rise] : slopes) {
        u32 tree_count = map.TreesInSlope(rise, run);
        product *= tree_count;
        fmt::print("Encountered {} trees using slope <{}, {}>\n", tree_count, run, rise);
    }
    fmt::print("Product = {}\n", product);
}
