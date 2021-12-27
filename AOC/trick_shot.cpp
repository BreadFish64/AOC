#include "types.hpp"

constexpr int xlo = 288;
constexpr int xhi = 330;
constexpr int ylo = -96;
constexpr int yhi = -50;

template <std::floating_point F>
inline std::pair<F, F> Quadratic(F a, F b, F c) {
    F radical = std::sqrt(b * b - 4 * a * c);
    return {(-b - radical) / 2, (-b + radical) / 2};
}

int min_vx = (int)std::ceil(Quadratic(1.0, 1.0, -2.0 * xlo).second);
constexpr int max_vx = xhi;
constexpr int min_vy = ylo;
constexpr int max_vy = -ylo - 1;

bool HitsTarget(int vx, int vy) {
    int x = 0, y = 0;
    while (x <= xhi && y >= ylo) {
        if (x >= xlo && y <= yhi) return true;
        y += vy;
        --vy;
        x += vx;
        if (--vx < 0) vx = 0;
    }
    return false;
}

int main() {
    fmt::print("{} <= vx <= {}\n", min_vx, max_vx);
    fmt::print("{} <= vy <= {}\n", min_vy, max_vy);
    fmt::print("search space = {}\n", (max_vy - min_vy + 1) * (max_vx - min_vx + 1));
    unsigned good_trajectories = 0;
    for (int vx = min_vx; vx <= max_vx; ++vx)
        for (int vy = min_vy; vy <= max_vy; ++vy) good_trajectories += HitsTarget(vx, vy);
    fmt::print("hits = {}\n", good_trajectories);
}
