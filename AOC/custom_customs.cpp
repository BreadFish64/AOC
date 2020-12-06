#include <algorithm>
#include <bitset>
#include <fstream>
#include <string>

#include <fmt/ostream.h>

#include "types.hpp"

int main() {
    std::ifstream input_file{"input.txt"};
    u64 any_count{0};
    u64 all_count{0};
    std::bitset<26> set_any = 0;
    std::bitset<26> set_all = ~0;
    std::string str;
    while (std::getline(input_file, str)) {
        if (str.empty()) {
            any_count += set_any.count();
            all_count += set_all.count();
            set_any = 0;
            set_all = ~0;
        } else {
            std::bitset<26> person_set = 0;
            for (char c : str) person_set.set(c - 'a');
            set_any |= person_set;
            set_all &= person_set;
        }
    }
    any_count += set_any.count();
    all_count += set_all.count();
    fmt::print("any: {}\nall: {}\n", any_count, all_count);
}