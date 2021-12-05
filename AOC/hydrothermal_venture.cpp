#include "types.hpp"

constexpr int sgn(int val) { return (0 < val) - (val < 0); }
struct Line {
    int x1, y1, x2, y2;
    int MaxX() const { return std::max(x1, x2); }
    int MaxY() const { return std::max(y1, y2); }
};
std::istream& operator>>(std::istream& stream, Line& line) {
    stream >> line.x1;
    stream.ignore(1);
    stream >> line.y1;
    stream.ignore(4);
    stream >> line.x2;
    stream.ignore(1);
    stream >> line.y2;
    return stream;
}
int main() {
    std::ifstream input_file{"input.txt"};
    std::vector input(std::istream_iterator<Line>{input_file}, {});

    const std::size_t width = std::ranges::max(input, std::less{}, &Line::MaxX).MaxX() + 1_sz;
    const std::size_t height = std::ranges::max(input, std::less{}, &Line::MaxY).MaxY() + 1_sz;
    std::vector<u16> sea_floor(height * width);
    const auto Cell = [&](int y, int x) -> u16& { return sea_floor[y * width + x]; };

    unsigned overlaps = 0;
    unsigned diagonal_lines = 0;
    const auto DrawLine = [&](const Line& line) {
        const int x_direction = sgn(line.x2 - line.x1);
        const int y_direction = sgn(line.y2 - line.y1);
        const int length = std::max(std::abs(line.x2 - line.x1), std::abs(line.y2 - line.y1));
        for (int i = 0, x = line.x1, y = line.y1; i <= length;
             ++i, x += x_direction, y += y_direction) {
            auto& vent = Cell(y, x);
            if (vent++ == 1) ++overlaps;
        }
    };
    for (const auto& line : input) {
        if (line.x1 == line.x2 || line.y1 == line.y2) {
            DrawLine(line);
        } else {
            input[diagonal_lines++] = line;
        }
    }
    input.resize(diagonal_lines);
    fmt::print("P1 {}\n", overlaps);

    for (const auto& line : input) DrawLine(line);
    fmt::print("P2 {}\n", overlaps);
}
