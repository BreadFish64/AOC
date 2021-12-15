#include "types.hpp"

struct Node {
    std::vector<Node*> edges;
    int visited;
};
unsigned CountPathsRecurse(const Node* const start, const Node* const end, Node* const current,
                      bool used_double) {
    if (current == end) return 1;
    int was_visited = current->visited;
    if (was_visited < 0) {
        if (used_double || current == start) return 0;
        used_double = true;
    } else {
        current->visited = -was_visited;
    }
    unsigned path_count = 0;
    for (Node* sub_node : current->edges)
        path_count += CountPathsRecurse(start, end, sub_node, used_double);
    current->visited = was_visited;
    return path_count;
}
void CountPaths(Node* start, const Node* end, bool allow_double) {
    auto start_time = std::chrono::steady_clock::now();
    const unsigned val = CountPathsRecurse(start, end, start, !allow_double);
    auto end_time = std::chrono::steady_clock::now();
    fmt::print("Paths: {}    Time: {}\n", val,
               std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time));
}
//int main() {
//    std::ifstream input_file{"input.txt"};
//    std::map<std::string, Node> nodes;
//    std::for_each(std::istream_iterator<std::string>{input_file}, {}, [&](const std::string& line) {
//        auto split = line.find('-');
//        auto& node_a = nodes[line.substr(0, split)];
//        auto& node_b = nodes[line.substr(split + 1)];
//        node_a.edges.emplace_back(&node_b);
//        node_b.edges.emplace_back(&node_a);
//    });
//    for (auto& [name, node] : nodes) node.visited = std::isupper(name[0]) ? 0 : 1;
//    Node* const start = &nodes.at("start");
//    const Node* const end = &nodes.at("end");
//    CountPaths(start, end, false);
//    CountPaths(start, end, true);
//}

#include <concepts>
#include <format>
#include <iostream>

void OutputValue(auto value) {
    static std::ostreambuf_iterator outerator{std::cout};
    std::format_to(outerator, "{:2}-bit value: {:d}\n", sizeof(value) * CHAR_BIT, value);
}

int main(int argc, char* argv[]) {
    uint8_t val1 = 55;
    uint16_t val2 = 32767;
    uint32_t val3 = 0xFF00FF;
    OutputValue(val1);
    OutputValue(val2);
    OutputValue(val3);
    return 0;
}
