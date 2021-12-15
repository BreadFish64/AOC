#include "types.hpp"

class DumboOctopus {
    using Octo = u16;
    static constexpr ssize width = 10;
    static constexpr ssize height = 10;
    static constexpr ssize stride = 16;
    static constexpr ssize offset = 32;
    std::array<Octo, offset + stride * height + stride> octopus_grid;

    u64 flashes = 0;
    u64 round_flashes = 0;
    u64 round = 0;

    std::chrono::steady_clock::time_point start_time;

    auto Row(ssize y) {
        return std::span<Octo, width>{octopus_grid.data() + offset + stride * y, width};
    }
    auto Row(ssize y) const {
        return std::span<const Octo, width>{octopus_grid.data() + offset + stride * y, width};
    }
    Octo& Octopus(ssize y, ssize x) { return octopus_grid[offset + stride * y + x]; }

    void RecursiveFlash(ssize y, ssize x) {
        if (++Octopus(y, x) != 10) return;
        RecursiveFlash(y - 1, x - 1);
        RecursiveFlash(y - 1, x);
        RecursiveFlash(y - 1, x + 1);
        RecursiveFlash(y, x - 1);
        RecursiveFlash(y, x + 1);
        RecursiveFlash(y + 1, x - 1);
        RecursiveFlash(y + 1, x);
        RecursiveFlash(y + 1, x + 1);
    }

public:
    explicit DumboOctopus(std::istream& input_stream) {
        std::ranges::fill(octopus_grid, 10);
        auto iter = std::istream_iterator<u8>{input_stream};
        for (ssize y = 0; y < height; ++y) {
            for (Octo& octopus : Row(y)) octopus = *iter++ - '0';
        }
    }
    void PrintState() const {
        const auto time = std::chrono::steady_clock::now();
        fmt::print("\n\nRound: {}    Flashes: {}    Total: {}    Time: {}\n", round, round_flashes,
                   flashes,
                   std::chrono::duration_cast<std::chrono::microseconds>(time - start_time));
        for (ssize y = 0; y < height; ++y) fmt::print("{}\n", Row(y));
    }
    void RunRound() {
        for (ssize y = 0; y < height; ++y) {
            for (ssize x = 0; x < width; ++x) RecursiveFlash(y, x);
        }
        u8 round_flashes = 0;
        for (ssize y = 0; y < height; ++y)
            for (Octo octopus : Row(y))
                if (octopus >= 10) ++round_flashes;
        for (ssize y = 0; y < height; ++y) {
            for (Octo& octopus_ref : Row(y)) {
                Octo octopus = octopus_ref;
                octopus_ref = octopus >= 10 ? 0 : octopus;
            }
        }
        this->round_flashes = round_flashes;
        flashes += round_flashes;
    }
    void Run() {
        const u64 area = width * height;
        start_time = std::chrono::steady_clock::now();
        while (round_flashes != area) {
            // while (round < 100) {
            ++round;
            RunRound();
        }
        PrintState();
    }
};

#include <Windows.h>

int main() {

    std::ifstream input_file{"input.txt"};
    DumboOctopus dumbo_octopus{input_file};
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    dumbo_octopus.Run();
}
