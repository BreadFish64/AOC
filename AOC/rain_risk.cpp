#include <fstream>

#include <fmt/ostream.h>

#include "types.hpp"

struct Direction {
    char instruction;
    unsigned val;

    friend std::istream& operator>>(std::istream& stream, Direction& direction) {
        stream >> direction.instruction;
        stream >> direction.val;
        return stream;
    }
};

struct Point {
    signed x{0}, y{0};
};

struct AbsoluteState {
    Point position;
    unsigned facing{0};

    static constexpr std::string_view cardinal = "ESWN"sv;

    void Apply(Direction direction) {
        switch (direction.instruction) {
        case 'N': position.y += direction.val; break;
        case 'S': position.y -= direction.val; break;
        case 'E': position.x += direction.val; break;
        case 'W': position.x -= direction.val; break;
        case 'L': facing = (facing - direction.val / 90) % 4; break;
        case 'R': facing = (facing + direction.val / 90) % 4; break;
        case 'F': {
            direction.instruction = cardinal[facing];
            Apply(direction);
        } break;
        default: std::abort();
        }
    }
};

struct WaypointState {
    Point waypoint{10, 1};
    Point ship;

    void Rotate(unsigned val) {
        switch (val) {
        case 0: break;
        case 90: {
            std::swap(waypoint.x, waypoint.y);
            waypoint.x = -waypoint.x;
        } break;
        case 180: {
            waypoint.x = -waypoint.x;
            waypoint.y = -waypoint.y;
        } break;
        case 270: {
            std::swap(waypoint.x, waypoint.y);
            waypoint.y = -waypoint.y;
        } break;
        case 360: break;
        default: std::abort();
        }
    }

    void Apply(Direction direction) {
        switch (direction.instruction) {
        case 'N': waypoint.y += direction.val; break;
        case 'S': waypoint.y -= direction.val; break;
        case 'E': waypoint.x += direction.val; break;
        case 'W': waypoint.x -= direction.val; break;
        case 'L': Rotate(direction.val); break;
        case 'R': Rotate(360 - direction.val); break;
        case 'F': {
            ship.x += waypoint.x * direction.val;
            ship.y += waypoint.y * direction.val;
        } break;
        default: std::abort();
        }
    }
};

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<Direction> directions{std::istream_iterator<Direction>{input_file}, {}};

    {
        AbsoluteState state;
        for (const auto& direction : directions) state.Apply(direction);

        auto [x, y] = state.position;
        fmt::print("Absolute\n");
        fmt::print("Position: {} {}\n", x, y);
        fmt::print("Manhattan Distance: {}\n\n", (x > 0 ? x : -x) + (y > 0 ? y : -y));
    }
    {
        WaypointState state;
        for (const auto& direction : directions) state.Apply(direction);

        auto [x, y] = state.ship;
        fmt::print("Waypoint\n");
        fmt::print("Position: {} {}\n", x, y);
        fmt::print("Manhattan Distance: {}\n\n", (x > 0 ? x : -x) + (y > 0 ? y : -y));
    }
}
