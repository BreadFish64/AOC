#include "types.hpp"

constexpr unsigned compress(unsigned a, unsigned b) { return a << 5 | b; }
constexpr std::pair<unsigned, unsigned> expand(unsigned x) { return {x >> 5, x & 0x1F}; }

int main() {
    std::ifstream input_file{"input.txt"};
    std::istream& input = input_file;

    std::array<u8, 1 << 10> insertion_rules{};
    std::array<usize, 1 << 10> pair_counts{};

    std::string polymer;
    input >> polymer;
    for (char& c : polymer) c -= 'A';
    // Setup initial count of each adjacent pair in the polymer
    for (auto a = polymer.begin(), b = a + 1; b < polymer.end(); ++a, ++b)
        ++pair_counts[compress(*a, *b)];

    // Setup the insertion rules
    std::string line;
    std::getline(input, line);
    std::getline(input, line);
    while (std::getline(input, line))
        insertion_rules[compress(line[0] - 'A', line[1] - 'A')] = line[6] - 'A';

    for (int iteration = 1; iteration <= 40; ++iteration) {
        // Run expansion iteration
        std::array<usize, 1 << 10> new_pair_counts{};
        for (unsigned pair = 0; pair < pair_counts.size(); ++pair) {
            const usize count = pair_counts[pair];
            if (!count) continue;
            const auto [a, c] = expand(pair);
            const u8 b = insertion_rules[pair];
            new_pair_counts[compress(a, b)] += count;
            new_pair_counts[compress(b, c)] += count;
        }
        pair_counts = new_pair_counts;

        // Print state
        std::array<usize, 32> counts{};
        ++counts[polymer.front()]; // compensate for the fact that every other element
        ++counts[polymer.back()];  // is counted in two pairs
        for (unsigned pair = 0; pair < pair_counts.size(); ++pair) {
            const auto [a, b] = expand(pair);
            counts[a] += pair_counts[pair];
            counts[b] += pair_counts[pair];
        }
        const usize most = std::ranges::max(counts);
        const usize least = std::ranges::min(counts, std::less{}, [](usize count) {
            return count ? count : std::numeric_limits<usize>::max();
        });
        fmt::print("iteration: {}    most - least: {}\n", iteration, (most - least) / 2);
    }
}
