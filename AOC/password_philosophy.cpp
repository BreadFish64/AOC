#include <algorithm>
#include <fstream>
#include <numeric>
#include <ranges>
#include <vector>

#include <fmt/ostream.h>

#include "types.hpp"

struct Entry {
    u32 min;
    u32 max;
    char letter;
    std::string password;

    friend std::istream& operator>>(std::istream& input, Entry& entry) {
        input >> entry.min;
        // skip "-"
        input.ignore(1);
        input >> entry.max;
        input >> entry.letter;
        // skip ": "
        input.ignore(2);
        input >> entry.password;
        return input;
    }

    bool VerifyRuleOne() const {
        auto count = std::ranges::count(password, letter);
        return count >= min && count <= max;
    }

    bool VerifyRuleTwo() const {
        return (password[min - 1] == letter) ^ (password[max - 1] == letter);
    }
};

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<Entry> data_set{std::istream_iterator<Entry>{input_file}, {}};
    u32 valid_passwords = std::ranges::count_if(data_set, &Entry::VerifyRuleOne);
    fmt::print("{} valid passwords according to rule one\n", valid_passwords);
    valid_passwords = std::ranges::count_if(data_set, &Entry::VerifyRuleTwo);
    fmt::print("{} valid passwords according to rule two\n", valid_passwords);
}
