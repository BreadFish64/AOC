#include "types.hpp"

constexpr auto GRID_SIDE = 5;
using Grid = std::array<std::array<u8, GRID_SIDE>, GRID_SIDE>;
struct GridPos {
    u16 grid;
    u8 y, x;
};
struct GridTracker {
    std::array<std::bitset<GRID_SIDE>, GRID_SIDE> row_tracker;
    std::array<std::bitset<GRID_SIDE>, GRID_SIDE> column_tracker;

    // return true if the grid has won
    bool Mark(u8 y, u8 x) {
        row_tracker[y][x] = true;
        column_tracker[x][y] = true;
        return row_tracker[y].all() || column_tracker[x].all();
    }

    unsigned SumUnmarked(const Grid& grid) const {
        unsigned sum = 0;
        for (unsigned y = 0; y < grid.size(); ++y) {
            const auto& row = grid[y];
            const auto row_marks = row_tracker[y];
            for (unsigned x = 0; x < row.size(); ++x) {
                if (!row_marks[x]) sum += row[x];
            }
        }
        return sum;
    }
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
    // Create a lookup table of possible calls to grid cells
    std::array<std::vector<GridPos>, 100> num_mappings;
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
    const auto PrintGrid = [&](unsigned grid_index, unsigned call) {
        const auto& grid = grids[grid_index];
        const auto& tracker = grid_trackers[grid_index];

        fmt::print("grid {}\n", grid_index);
        for (const auto& row : grid) fmt::print("{}\n", row);
        fmt::print("tracker grid\n");
        for (const auto row : tracker.row_tracker) fmt::print("{}\n", row);
        const auto unmarked_sum = tracker.SumUnmarked(grid);
        fmt::print("unmarked sum: {}\ncall: {}\nresult: {}\n\n", unmarked_sum, call,
                   unmarked_sum * call);
    };
    // Keep track of which sets have won
    std::set<u16> grids_left;
    for (unsigned i = 0; i < grids.size(); ++i) grids_left.emplace_hint(grids_left.end(), i);
    // Keep track of whether any set has won
    bool won = false;
    for (auto call : calls) {
        const auto& positions = num_mappings[call];
        for (auto [grid, y, x] : positions) {
            if (grid_trackers[grid].Mark(y, x)) {
                if (!won) {
                    won = true;
                    fmt::print("winner:\n");
                    PrintGrid(grid, call);
                }
                grids_left.erase(grid);
                if (grids_left.empty()) {
                    fmt::print("loser:\n");
                    PrintGrid(grid, call);
                    return 0;
                }
            }
        }
    }
    return 1;
}
