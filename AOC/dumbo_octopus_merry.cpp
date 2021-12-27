#include <array>
#include <bit>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <string>

using u64 = std::uint64_t;
using Board = std::array<u64, 2>;
using State = std::array<Board, 10>;

inline constexpr u64 rep5(u64 mask) { return mask | (mask << 10) | (mask << 20) | (mask << 30) | (mask << 40); }

bool isempty(Board b) { return (b[0] == 0) & (b[1] == 0); }
Board rotr(Board b) { return { (b[0] >> 1) & rep5(0b01111'11111), (b[1] >> 1) & rep5(0b01111'11111) }; }
Board rotl(Board b) { return { (b[0] << 1) & rep5(0b11111'11110), (b[1] << 1) & rep5(0b11111'11110) }; }
Board rotd(Board b) { return { (b[0] >> 10) | (((b[1] << (64 - 10)) >> (64 - 10)) << 40), (b[1] >> 10) }; }
Board rotu(Board b) { return { (b[0] << 10) & rep5(0b11111'11111), ((b[1] << 10) | (b[0] >> 40)) & rep5(0b11111'11111)}; }

Board operator&(Board a, Board b) { return { a[0] & b[0], a[1] & b[1] }; }
Board operator|(Board a, Board b) { return { a[0] | b[0], a[1] | b[1] }; }
Board operator~(Board a) { return { ~a[0], ~a[1] }; }
void set_bit(Board& b, size_t i, size_t j) {
    const size_t index = i * 10 + j;
    b[index / 50] |= u64(1) << (index % 50);
}
bool get_bit(Board& b, size_t i, size_t j) {
    const size_t index = i * 10 + j;
    return (b[index / 50] >> (index % 50)) & 1;
}
size_t count(Board b) {
    return std::popcount(b[0]) + std::popcount(b[1]);
}

void dump_state(State& s) {
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++)
            for (size_t o = 0; o < 10; o++)
                if (get_bit(s[o], i, j))
                    printf("%zu", o);
        printf("\n");
    }
    printf("\n");
}

void dump_board(Board b) {
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++)
            printf("%c", get_bit(b, i, j) ? '1' : '.');
        printf("\n");
    }
    printf("\n");
}

void prop(State& s, Board flash, Board& total_flash);

void bubble(State& s, Board flash, Board& total_flash) {
    Board to_up{0, 0};
    for (size_t i = 0; i < 10; i++) {
        s[i] = s[i] | to_up;
        to_up = s[i] & flash;
        s[i] = s[i] & ~to_up;
        flash = flash & ~to_up;
    }
    to_up = to_up & ~total_flash;
    total_flash = total_flash | to_up;

    if (!isempty(to_up))
        prop(s, to_up, total_flash);
}

void prop(State& s, Board flash, Board& total_flash) {
    bubble(s, rotr(flash), total_flash);
    bubble(s, rotl(flash), total_flash);
    bubble(s, rotd(flash), total_flash);
    bubble(s, rotu(flash), total_flash);
    bubble(s, rotr(rotd(flash)), total_flash);
    bubble(s, rotl(rotd(flash)), total_flash);
    bubble(s, rotr(rotu(flash)), total_flash);
    bubble(s, rotl(rotu(flash)), total_flash);
}

size_t step(State& s) {
    Board flash = s[9];
    for (size_t i = 0; i < 9; i++) s[9 - i] = s[9 - i - 1];
    s[0] = {0, 0};

    Board total_flash = flash;
    prop(s, flash, total_flash);
    s[0] = total_flash;
    return count(total_flash);
}

State read_state() {
    State result{};

    std::ifstream input_file{"input.txt"};
    for (size_t i = 0; i < 10; i++) {
        std::string line;
        std::getline(input_file, line);
        for (size_t j = 0; j < 10; j++)
            set_bit(result[line[j] - '0'], i, j);
        printf("input: %s\n", line.c_str());
    }

    return result;
}

int main() {
    State s = read_state();

    const auto start_time = std::chrono::steady_clock::now();

    size_t flash_count = 0;
    for (size_t i=0; i<100; i++) {
        flash_count += step(s);
    }

    const auto end_time = std::chrono::steady_clock::now();
    std::printf("count: %zu, time: %lldus", flash_count, std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());
}
