#include "types.hpp"

struct Grid {
    std::vector<char> data;
    std::vector<char> temp;

    ssize width{};
    ssize stride{};
    ssize height{};

    u64 steps = 0;

    char& Cell(std::span<char> grid, ssize y, ssize x) {
        if (y >= height)
            y -= height;
        else if (y < 0)
            y += height;

        if (x >= width)
            x -= width;
        else if (y < 0)
            x += width;

        return grid[y * stride + x];
    }

public:
    Grid(std::istream& stream) {
        data = std::vector(std::istreambuf_iterator{stream}, {});

        width = std::ranges::find(data, '\n') - data.begin();
        stride = width + 1;
        height = data.size() / stride;

        temp = data;
    }

    u64 GetSteps() { return steps; }

    bool Step() {
        fmt::print("{} steps\n", steps);
        for (ssize y = 0; y < height; ++y) {
            fmt::print("{}\n", std::string_view{data.data() + y * stride, static_cast<usize>(width)});
        }

        ++steps;

        bool moved = false;

        temp = data;
        for (ssize y = 0; y < height; ++y) {
            for (ssize x = 0; x < width; ++x) {
                char& temp_cell = Cell(temp, y, x);
                char cell = Cell(data, y, x);
                char right_cell = Cell(data, y, x + 1);
                if (cell == '>' && right_cell == '.') {
                    temp_cell = '.';
                    Cell(temp, y, x + 1) = '>';
                    moved = true;
                }
            }
        }
        data = temp;
        for (ssize y = 0; y < height; ++y) {
            for (ssize x = 0; x < width; ++x) {
                char& cell = Cell(data, y, x);
                char temp_cell = Cell(temp, y, x);
                char bottom_cell = Cell(temp, y + 1, x);
                if (temp_cell == 'v' && bottom_cell == '.') {
                    cell = '.';
                    Cell(data, y + 1, x) = 'v';
                    moved = true;
                }
            }
        }

        return moved;
    }
};

int main() {
    std::ifstream input_file{"input.txt"};
    std::istringstream test{R"(v...>>.vv>
.vv>>.vv..
>>.>v>...v
>>v>>.>.v.
v>v.vv.v..
>.>>..v...
.vv..>.>v.
v.v..>>v.v
....v..v.>
)"};
    Grid grid{input_file};
    while (grid.Step())
        ;
    fmt::print("{} steps\n", grid.GetSteps());
}