#include "types.hpp"

char ClosingBracket(char c) {
    switch (c) {
    case '(': return ')';
    case '[': return ']';
    case '{': return '}';
    case '<': return '>';
    default: return 0;
    }
}
u64 CorruptionScore(char c) {
    switch (c) {
    case ')': return 3;
    case ']': return 57;
    case '}': return 1197;
    case '>': return 25137;
    default: throw 1;
    }
}
u64 AutocompleteScore(std::span<char> bracket_stack) {
    u64 autocomplete_score = 0;
    for (char c : std::ranges::reverse_view{bracket_stack}) {
        autocomplete_score *= 5;
        switch (c) {
        case ')': autocomplete_score += 1; break;
        case ']': autocomplete_score += 2; break;
        case '}': autocomplete_score += 3; break;
        case '>': autocomplete_score += 4; break;
        default: throw 2;
        }
    }
    return autocomplete_score;
}
std::pair<u64, u64> ScoreLine(std::string_view line) {
    std::vector<char> bracket_stack;
    for (char c : line) {
        char closing_bracket = ClosingBracket(c);
        if (closing_bracket) {
            bracket_stack.push_back(closing_bracket);
        } else {
            if (bracket_stack.back() != c) return {CorruptionScore(c), 0};
            bracket_stack.pop_back();
        }
    }
    return {0, AutocompleteScore(bracket_stack)};
}
int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<std::string> input(std::istream_iterator<std::string>{input_file}, {});
    u64 corrupted_score = 0;
    std::vector<u64> autocomplete_scores;
    for (const auto& line : input) {
        auto [corrupt, autocomplete] = ScoreLine(line);
        corrupted_score += corrupt;
        if (autocomplete) autocomplete_scores.emplace_back(autocomplete);
    }
    std::ranges::sort(autocomplete_scores);
    u64 autocomplete_score = autocomplete_scores[autocomplete_scores.size() / 2];
    fmt::print("P1: {}\nP2: {}\n", corrupted_score, autocomplete_score);
}
