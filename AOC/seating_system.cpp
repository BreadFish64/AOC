#include <bitset>
#include <fstream>

#include <fmt/ostream.h>

#include "types.hpp"

struct Board {
    usize width;
    usize height;
    std::string raw;
    char* operator[](s64 row) { return raw.data() + (row + 1) * (width + 2) + 1; }
    const char* operator[](s64 row) const { return raw.data() + (row + 1) * (width + 2) + 1; }
    bool operator==(const Board& rhs) const = default;

    Board() = default;
    Board(const Board&) = default;
    Board(std::istream& input) {
        std::string row;
        std::getline(input, row);
        width = row.size();
        height = 1;
        raw = std::string(width + 3, '.');
        raw += row;
        raw += '.';
        while (std::getline(input, row)) {
            raw += '.';
            raw += row;
            raw += '.';
            ++height;
        }
    }

    bool InBounds(s64 x, s64 y) const { return x >= 0 && y >= 0 && x < width && y < height; }

    u64 Occupants() const { return std::ranges::count(raw, '#'); }

    void Print() const {
        for (usize y{0}; y < height; ++y) fmt::print("{}\n", std::string_view{(*this)[y], width});
        fmt::print("\n");
    }
};

class Game {
    u8 current_board = 0;
    std::array<Board, 2> boards;

    static constexpr std::initializer_list<std::pair<s64, s64>> sample_offsets{
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
    };

    void RunGeneration1() {
        const Board& from = boards[current_board];
        Board& to = boards[current_board ^ 1];
        for (s64 y{0}; y < from.height; ++y) {
            for (s64 x{0}; x < from.width; ++x) {
                char cc = from[y][x];
                if (cc == 'L') {
                    bool personal_space{true};
                    for (auto [dy, dx] : sample_offsets)
                        if (from[y + dy][x + dx] == '#') personal_space = false;
                    to[y][x] = personal_space ? '#' : cc;
                } else if (cc == '#') {
                    unsigned neighbors{0};
                    for (auto [dy, dx] : sample_offsets)
                        if (from[y + dy][x + dx] == '#') ++neighbors;
                    to[y][x] = neighbors >= 4 ? 'L' : cc;
                } else {
                    to[y][x] = cc;
                }
            }
        }
        current_board ^= 1;
    }

    void RunGeneration2() {
        const Board& from = boards[current_board];
        Board& to = boards[current_board ^ 1];
        for (s64 y{0}; y < from.height; ++y) {
            for (s64 x{0}; x < from.width; ++x) {
                char cc = from[y][x];
                if (cc == 'L') {
                    for (auto [dy, dx] : sample_offsets) {
                        auto ty = y + dy, tx = x + dx;
                        while (from[ty][tx] == '.' && from.InBounds(tx, ty)) {
                            ty += dy;
                            tx += dx;
                        }
                        if (from[ty][tx] == '#') {
                            to[y][x] = cc;
                            goto next_cell;
                        }
                    }
                    to[y][x] = '#';
                } else if (cc == '#') {
                    unsigned neighbors{0};
                    for (auto [dy, dx] : sample_offsets) {
                        auto ty = y + dy, tx = x + dx;
                        while (from[ty][tx] == '.' && from.InBounds(tx, ty)) {
                            ty += dy;
                            tx += dx;
                        }
                        if (from[ty][tx] == '#') {
                            ++neighbors;
                            if (neighbors >= 5) {
                                to[y][x] = 'L';
                                goto next_cell;
                            }
                        }
                    }
                    to[y][x] = cc;
                } else {
                    to[y][x] = cc;
                }
            next_cell:;
            }
        }
        current_board ^= 1;
    }

public:
    Game(const Game&) = default;
    Game(std::istream& input) {
        boards[0] = Board{input};
        boards[1] = boards[0];
    }

    u64 StableOccupation1() {
        do {
            RunGeneration1();
            // boards[current_board].Print();
        } while (boards[0] != boards[1]);
        return boards[0].Occupants();
    }

    u64 StableOccupation2() {
        do {
            RunGeneration2();
            // boards[current_board].Print();
        } while (boards[0] != boards[1]);
        return boards[0].Occupants();
    }
};

int main() {
    std::ifstream input_file{"input.txt"};
    Game game1{input_file};
    Game game2 = game1;

    u64 occupants1 = game1.StableOccupation1();
    u64 occupants2 = game2.StableOccupation2();
    fmt::print("Occupants 1: {}\n", occupants1);
    fmt::print("Occupants 2: {}\n", occupants2);
    return 0;
}