#include "types.hpp"

struct Paper {
    usize height{};
    usize width{};
    std::vector<u8> storage;

    explicit Paper() = default;
    explicit Paper(usize height, usize width)
        : height{height}, width{width}, storage(width * height, false) {}
    std::span<u8> operator[](usize y) { return std::span{storage}.subspan(y * width, width); }

    void PrintStats() {
        const usize dots = std::reduce(storage.begin(), storage.end(), usize{0});
        fmt::print("{}x{}   dots: {}\n", width, height, dots);
    }
    void PrintPaper() {
        for (usize y = 0; y < height; ++y) { fmt::print("{}\n", (*this)[y]); }
    }
    Paper Fold(bool horizontal, usize line) {
        Paper new_paper;
        if (horizontal) {
            const usize rev_line = height - line - 1;
            new_paper = Paper{std::max(line, rev_line), width};
            for (usize x = 0; x < width; ++x) {
                for (usize y = 0; y < line; ++y) {
                    new_paper[new_paper.height - line + y][x] |= (*this)[y][x];
                }
                for (usize y = 0; y < rev_line; ++y) {
                    new_paper[new_paper.height - rev_line + y][x] |= (*this)[height - 1 - y][x];
                }
            }
        } else {
            const usize rev_line = width - line - 1;
            new_paper = Paper{height, std::max(line, rev_line)};
            for (usize y = 0; y < height; ++y) {
                for (usize x = 0; x < line; ++x) {
                    new_paper[y][new_paper.width - line + x] |= (*this)[y][x];
                }
                for (usize x = 0; x < rev_line; ++x) {
                    new_paper[y][new_paper.width - rev_line + x] |= (*this)[y][width - 1 - x];
                }
            }
        }
        new_paper.PrintStats();
        return new_paper;
    }
};

int main() {
    std::vector<std::pair<usize, usize>> dots;
    std::vector<std::pair<bool, usize>> folds;
    {
        std::ifstream input_file{"input.txt"};
        std::string line;
        while (std::getline(input_file, line) && !line.empty()) {
            const auto comma = line.find(',');
            dots.emplace_back(std::stoull(line.substr(comma + 1)),
                              std::stoull(line.substr(0, comma)));
        }
        while (std::getline(input_file, line)) {
            folds.emplace_back(line[11] == 'y', std::stoull(line.substr(13)));
        }
    }
    const usize width =
        std::ranges::max(dots, std::less{}, [](auto pair) { return pair.second; }).second + 1;
    const usize height =
        std::ranges::max(dots, std::less{}, [](auto pair) { return pair.first; }).first + 1;

    Paper paper{height, width};
    for (const auto [y, x] : dots) paper[y][x] = true;

    paper.PrintStats();
    for (const auto [horizontal, line] : folds) paper = paper.Fold(horizontal, line);
    paper.PrintPaper();
}
