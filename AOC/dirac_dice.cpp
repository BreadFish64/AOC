#include "types.hpp"

class DeterministicDice {
    u64 val = 0;
    u64 rolls = 0;

public:
    u64 Roll() {
        ++rolls;
        auto v = val + 1;
        val = v % 100;
        return v;
    }

    u64 RollThree() { return Roll() + Roll() + Roll(); }

    u64 GetRolls() const { return rolls; }
};

struct GameState {
    std::array<u64, 2> players;
    std::array<u64, 2> scores{0, 0};
    unsigned current_player = 0;

    bool RunTurn(u64 roll) {
        auto& pos = players[current_player];
        auto& score = scores[current_player];
        current_player = !current_player;
        pos = (pos + roll) % 10;
        score += pos + 1;
        return score >= 21;
    }
};

constexpr std::array<u64, 10> weights = [] {
    std::array<u64, 10> arr{};
    for (int a = 1; a <= 3; ++a)
        for (int b = 1; b <= 3; ++b)
            for (int c = 1; c <= 3; ++c) ++arr[a + b + c];
    return arr;
}();

std::array<u64, 2> RecursiveDiracSim(GameState g, u64 roll) {
    std::array<u64, 2> loss{};
    if (g.RunTurn(roll)) {
        loss[g.current_player] = 1;
        return loss;
    }
    for (int d = 3; d <= 9; ++d) {
        auto [a, b] = RecursiveDiracSim(g, d);
        loss[0] += weights[d] * a;
        loss[1] += weights[d] * b;
    }
    return loss;
}

u64 DiracSim() {
    GameState g{
        .players{10, 3},
    };
    for (auto& player : g.players) --player;
    std::array<u64, 2> loss{};
    for (int d = 3; d <= 9; ++d) {
        auto [a, b] = RecursiveDiracSim(g, d);
        loss[0] += weights[d] * a;
        loss[1] += weights[d] * b;
    }

    return std::ranges::max(loss);
}

int main() {
    std::array<u64, 2> players{10, 3};
    for (auto& player : players) --player;

    std::array<u64, players.size()> score{0, 0};
    DeterministicDice dice;
    while (true) {
        for (usize p = 0; p < players.size(); ++p) {
            fmt::print("P{}: pos: {} score: {}\n", p, players[p] + 1, score[p]);
            u64 pos = players[p];
            pos += dice.RollThree();
            pos %= 10;
            players[p] = pos;
            score[p] += pos + 1;
            if (score[p] >= 1000) goto game_over;
        }
    }
game_over:
    fmt::print("Winner: {} Loser: {} Rolls: {} P1: {}\n", std::ranges::max(score),
               std::ranges::min(score), dice.GetRolls(), std::ranges::min(score) * dice.GetRolls());

    fmt::print("Part 2: {}\n", DiracSim());
}