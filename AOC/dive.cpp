#include "types.hpp"

int main() {
    std::ifstream input_file{"input.txt"};
    std::string direction;
    int distance = 0, depth = 0, move{}, aim = 0; // aim happens to be the same as the part 1 depth
    while (!input_file.eof()) {
        input_file >> direction >> move;
        switch (direction[0]) {
        case 'f': distance += move; depth += move * aim; break;
        case 'd': aim += move; break;
        case 'u': aim -= move; break;
        default: break;
        }
    }
    fmt::print("P1 depth: {}\nP2 depth: {}", distance * aim, distance * depth);
}
