#include <bit>
#include <bitset>
#include <fstream>
#include <string>
#include <unordered_map>

#include <fmt/ostream.h>

#include "types.hpp"

using Native = std::bitset<36>;

void RecurseFloatingAddr(Native floating_mask, Native addr,
                       const std::function<void(Native)>& callback) {
    if (floating_mask.none()) {
        callback(addr);
        return;
    }
    unsigned bit = std::countr_zero(floating_mask.to_ullong());
    floating_mask.reset(bit);
    RecurseFloatingAddr(floating_mask, addr.reset(bit), callback);
    RecurseFloatingAddr(floating_mask, addr.set(bit), callback);
};

int main() {
    std::ifstream file{"input.txt"};
    std::unordered_map<Native, Native> memory_a;
    std::unordered_map<Native, Native> memory_b;
    std::string line;
    Native one_mask{0};
    Native zero_mask{0};
    Native floating_mask{0};
    while (std::getline(file, line)) {
        if (line[1] == 'a') {
            one_mask = 0;
            zero_mask = 0;
            floating_mask = 0;
            auto it = line.cbegin() + 7;
            for (int bit{35}; bit >= 0; --bit) {
                switch (*it++) {
                case '0': zero_mask.set(bit); break;
                case '1': one_mask.set(bit); break;
                case 'X': floating_mask.set(bit); break;
                default: std::abort();
                }
            }
        } else {
            char* pos = line.data() + 4;
            Native addr = std::strtoull(pos, &pos, 10);
            Native val = std::strtoull(pos + 4, &pos, 10);
            memory_a[addr] = val | one_mask & ~zero_mask;
            RecurseFloatingAddr(floating_mask, addr | one_mask,
                        [&memory_b, val](Native addr) { memory_b[addr] = val; });
        }
    }
    {
        u64 sum{0};
        for (auto [addr, val] : memory_a) sum += val.to_ullong();
        fmt::print("Sum A: {}\n", sum);
    }
    {
        u64 sum{0};
        for (auto [addr, val] : memory_b) sum += val.to_ullong();
        fmt::print("Sum B: {}\n", sum);
    }
}
