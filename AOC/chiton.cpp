#include "types.hpp"

constexpr usize TILE_SIZE = 100;
constexpr usize TILES = 5;
constexpr usize SIZE = TILE_SIZE * TILES;
constexpr usize MAX_IDX = SIZE - 1;
constexpr usize stride = TILE_SIZE + 1;

int main() {
    std::basic_ifstream<u8> input_file{"input.txt"};
    std::vector<u8> input_grid(std::istreambuf_iterator<u8>{input_file}, {});

    auto start_time = std::chrono::steady_clock::now();
    // Create grid of risk values
    for (u8& risk : input_grid) risk -= '0';
    auto risk_grid_heap = std::make_unique<std::array<std::array<u8, SIZE>, SIZE>>();
    auto& risk_grid = *risk_grid_heap;
    for (usize y_tile = 0; y_tile < TILES; ++y_tile) {
        for (usize x_tile = 0; x_tile < TILES; ++x_tile) {
            const usize risk_offset = y_tile + x_tile;
            for (usize y = 0; y < TILE_SIZE; ++y) {
                const auto input_row = std::span{input_grid}.subspan(stride * y, TILE_SIZE);
                auto output_row = std::span{risk_grid[y_tile * TILE_SIZE + y]}.subspan(
                    x_tile * TILE_SIZE, TILE_SIZE);
                std::ranges::transform(input_row, output_row.begin(),
                                       [&](u8 risk) { return (risk - 1 + risk_offset) % 9 + 1; });
            }
        }
    }

    // Create initial path lengths
    auto path_len_heap = std::make_unique<std::array<std::array<usize, SIZE>, SIZE>>();
    auto& path_len = *path_len_heap;
    for (auto& row : path_len) std::ranges::fill(row, std::numeric_limits<usize>::max());
    path_len[0][0] = 0;

    // Recalculate the smallest distance to any point until it's stable
    bool computing = true;
    const auto Sample = [&](ssize y, ssize x) {
        return (x >= 0 && x < SIZE && y >= 0 && y < SIZE) ? path_len[y][x]
                                                          : std::numeric_limits<usize>::max();
    };
    while (computing) {
        computing = false;
        for (usize y = 0; y < SIZE; ++y) {
            for (usize x = 0; x < SIZE; ++x) {
                auto min_adjacent = std::min(
                    {Sample(y - 1, x), Sample(y, x - 1), Sample(y, x + 1), Sample(y + 1, x)});
                usize potential = min_adjacent + risk_grid[y][x];
                usize& current = path_len[y][x];
                if (potential < current) {
                    current = potential;
                    computing = true;
                }
            }
        }
    }
    auto end_time = std::chrono::steady_clock::now();
    fmt::print("{}\n{:.1}\n", path_len[MAX_IDX][MAX_IDX], std::chrono::duration<float, std::milli>(end_time - start_time));
}
