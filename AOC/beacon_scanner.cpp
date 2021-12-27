#include "types.hpp"

struct Point {
    int x, y, z;
};

struct Triangle {
    Point a, b, c;
    // using SideKey = std::array<std::array<int, 3>, 3>;
    // SideKey Key() const {
    //    const auto Side = [](const Point& u, const Point& v) {
    //        std::array<int, 3> side;
    //        side[0] = std::abs(u.x - v.x);
    //        side[1] = std::abs(u.y - v.y);
    //        side[2] = std::abs(u.z - v.z);
    //        std::ranges::sort(side);
    //        return side;
    //    };
    //    SideKey sides;
    //    sides[0] = Side(a, b);
    //    sides[1] = Side(b, c);
    //    sides[2] = Side(a, c);
    //    std::ranges::sort(sides);
    //    return sides;
    //}
    u64 Key() const {
        const auto Side = [](const Point& u, const Point& v) {
            u64 side = 0;
            side += std::abs(u.x - v.x);
            side += std::abs(u.y - v.y);
            side += std::abs(u.z - v.z);
            return side;
        };
        u64 sides = 1;
        sides *= Side(a, b);
        sides *= Side(b, c);
        sides *= Side(c, a);
        return sides;
    }
};

int main() {
    std::ifstream input{"input.txt"};
    std::vector<std::vector<Point>> scanners;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        if (std::ranges::count(line, '-') == 6) {
            scanners.emplace_back();
            continue;
        }
        Point p;
        std::sscanf(line.data(), "%d,%d,%d", &p.x, &p.y, &p.z);
        scanners.back().emplace_back(p);
    }
    std::set<u64> E;
    for (int i = 0; i < 1; ++i) {
        auto& scanner = scanners[i];
        std::map<u64, std::optional<Triangle>> unique_triangles;
        for (auto a = scanner.begin(); a != scanner.end(); ++a) {
            for (auto b = a + 1; b != scanner.end(); ++b) {
                for (auto c = b + 1; c != scanner.end(); ++c) {
                    Triangle t{*a, *b, *c};
                    auto [it, emplaced] = unique_triangles.try_emplace(t.Key(), t);
                    if (!emplaced) it->second.reset();
                }
            }
        }
        std::erase_if(unique_triangles, [](const auto& pair) { return !pair.second.has_value(); });
        for (const auto& [key, tri] : unique_triangles) fmt::print("{}\n", key);
    }
}