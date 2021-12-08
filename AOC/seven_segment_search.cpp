#include "types.hpp"

std::pair<unsigned, unsigned> SolveLine(std::istream& input) {
    std::array<std::bitset<7>, 10> patterns;
    std::array<std::bitset<7>, 10> digits;
    std::array<unsigned char, 1 << 7> mapping;
    std::string pattern_str;
    for (auto& pattern : patterns) {
        input >> pattern_str;
        for (char c : pattern_str) pattern[c - 'a'] = true;
    }
    const auto SetPattern = [&](std::bitset<7> pattern, unsigned digit) {
        digits[digit] = pattern;
        mapping[pattern.to_ullong()] = digit;
    };
    for (auto pattern : patterns) {
        switch (pattern.count()) {
        case 2: SetPattern(pattern, 1); break;
        case 3: SetPattern(pattern, 7); break;
        case 4: SetPattern(pattern, 4); break;
        case 7: SetPattern(pattern, 8); break; // technically don't need to check this
        default: break;
        }
    }
    for (auto pattern : patterns) {
        switch (pattern.count()) {
        case 5: {
            if ((pattern & digits[1]) == digits[1]) {
                SetPattern(pattern, 3);
            } else if ((pattern | digits[4]) == digits[8]) {
                SetPattern(pattern, 2);
            } else {
                SetPattern(pattern, 5);
            }
        } break;
        case 6: {
            if ((pattern & digits[4]) == digits[4]) {
                SetPattern(pattern, 9);
            } else if ((pattern & digits[1]) == digits[1]) {
                SetPattern(pattern, 0);
            } else {
                SetPattern(pattern, 6);
            }
        } break;
        default: break;
        }
    }
    input.ignore(3);
    unsigned output_num = 0;
    unsigned easy_digits = 0;
    for (int i = 0; i < 4; ++i) {
        input >> pattern_str;
        std::bitset<7> pattern;
        for (char c : pattern_str) pattern[c - 'a'] = true;
        unsigned digit = mapping[pattern.to_ullong()];
        output_num *= 10;
        output_num += digit;
        switch (digit) {
        case 1:
        case 4:
        case 7:
        case 8: ++easy_digits; break;
        default: break;
        }
    }
    return {easy_digits, output_num};
}

int main() {
    std::ifstream input_file{"input.txt"};
    std::istream& input = input_file;
    unsigned easy_digit_sum = 0;
    unsigned output_sum = 0;
    while (!input.eof()) {
        auto [easy_digits, output] =  SolveLine(input);
        easy_digit_sum += easy_digits;
        output_sum += output;
    }
    fmt::print("P1: {}\nP2: {}", easy_digit_sum, output_sum);
}
