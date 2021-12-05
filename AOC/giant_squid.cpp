#include "types.hpp"

#include <boost/container/small_vector.hpp>

#define EXECUTE

constexpr auto GRID_SIDE = 5;
using Grid = std::array<std::array<u8, GRID_SIDE>, GRID_SIDE>;
struct GridPos {
    u16 grid;
    u8 y, x;
};
struct GridTracker {
    // using u8 since bitset wasted 3 bytes
    static constexpr unsigned WIN_VAL = (1 << GRID_SIDE) - 1;
    bool won = false;
    std::array<u8, GRID_SIDE> row_tracker{};
    std::array<u8, GRID_SIDE> column_tracker{};

    GridTracker() {
        std::ranges::fill(row_tracker, WIN_VAL);
        std::ranges::fill(column_tracker, WIN_VAL);
    }

    // return true if the grid has just won
    bool Mark(u8 y, u8 x) {
        if (won) return false;
        const unsigned mask = ~(1 << x);
        won = !(row_tracker[y] &= mask);
        won |= !(column_tracker[x] &= mask);
        return won;
    }

    unsigned SumUnmarked(const Grid& grid) const {
        unsigned sum = 0;
        for (unsigned y = 0; y < grid.size(); ++y) {
            const auto& row = grid[y];
            const auto row_marks = row_tracker[y];
            for (unsigned x = 0; x < row.size(); ++x) {
                if (row_marks & (1 << x)) sum += row[x];
            }
        }
        return sum;
    }
};

void PrintGrid(const Grid& grid, const GridTracker& tracker, u8 call, unsigned unmarked_sum);

[[gnu::noinline]] auto Bingo(std::span<const Grid> grids, std::span<const u8> calls) {
    auto start_time = std::chrono::steady_clock::now();

    // Create a lookup table of possible calls to grid cells
    std::array<boost::container::small_vector<GridPos, 48>, 100> num_mappings;
    for (u16 g = 0; g < grids.size(); ++g) {
        const auto& grid = grids[g];
        for (u8 y = 0; y < grid.size(); ++y) {
            const auto& row = grid[y];
            for (u8 x = 0; x < row.size(); ++x) {
                auto num = row[x];
                num_mappings[num].emplace_back(GridPos{g, y, x});
            }
        }
    }

    std::vector<GridTracker> grid_trackers(grids.size());

    unsigned winning_grid = ~0u;
    unsigned winning_call = ~0u;
    unsigned losing_grid;
    unsigned losing_call;
    for (auto call : calls) {
        const auto& positions = num_mappings[call];
        for (auto [grid, y, x] : positions) {
            if (grid_trackers[grid].Mark(y, x)) {
                losing_grid = grid;
                losing_call = call;
                if (winning_grid == ~0u) {
                    winning_grid = grid;
                    winning_call = call;
                }
            }
        }
    }
    const unsigned winning_unmarked_sum =
        grid_trackers[winning_grid].SumUnmarked(grids[winning_grid]);
    const unsigned losing_unmarked_sum =
        grid_trackers[winning_grid].SumUnmarked(grids[winning_grid]);
    auto time = std::chrono::steady_clock::now() - start_time;
    PrintGrid(grids[winning_grid], grid_trackers[winning_grid], winning_call, winning_unmarked_sum);
    PrintGrid(grids[losing_grid], grid_trackers[losing_grid], losing_call, losing_unmarked_sum);
    return time;
}

#ifdef EXECUTE

#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <iostream>

[[gnu::noinline]] void PrintGrid(const Grid& grid, const GridTracker& tracker, u8 call,
                                 unsigned unmarked_sum) {
    fmt::print("grid\n");
    for (const auto& row : grid) fmt::print("{}\n", row);
    fmt::print("tracker grid\n");
    for (const auto row : tracker.row_tracker) fmt::print("{}\n", row);
    fmt::print("unmarked sum: {}\ncall: {}\nresult: {}\n\n", unmarked_sum, call,
               unmarked_sum * call);
};

int main() {
    std::ifstream input_file{"input.txt"};
    auto calls = [&] {
        // Parse top row of called numbers
        std::string top_line;
        std::getline(input_file, top_line);
        std::ranges::replace(top_line, ',', ' ');
        std::stringstream str{top_line};
        return std::vector<std::uint8_t>(std::istream_iterator<unsigned>{str}, {});
    }();
    // Read the grids
    std::vector<u8> grid_backing(std::istream_iterator<unsigned>{input_file}, {});
    std::span grids{reinterpret_cast<Grid*>(grid_backing.data()),
                    grid_backing.size() / sizeof(Grid)};
    auto time = Bingo(grids, calls);
    fmt::print("\nruntime: {}\n", time);
}

#endif