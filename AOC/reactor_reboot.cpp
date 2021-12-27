#include "types.hpp"

#include <scn/scn.h>

#include <boost/icl/map.hpp>
#include <boost/icl/split_interval_map.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

struct Box {
    using Side = boost::icl::closed_interval<s32>;

    std::array<Side, 3> bounds{};
    bool on{};

    bool Contains(const Box& other) const {
        for (usize d = 0; d < bounds.size(); ++d)
            if (!boost::icl::contains(bounds[d], other.bounds[d])) return false;
        return true;
    }
    friend auto operator<=>(const Box&, const Box&) = default;
};
constexpr auto x = sizeof(Box);

std::istream& operator>>(std::istream& stream, Box& box) {
    std::string line;
    std::getline(stream, line);
    std::string on_str;
    s32 x1, x2, y1, y2, z1, z2;
    scn::scan(line, "{} x={}..{},y={}..{},z={}..{}", on_str, x1, x2, y1, y2, z1, z2);
    box = Box{
        .bounds{
            Box::Side{x1, x2},
            Box::Side{y1, y2},
            Box::Side{z1, z2},
        },
        .on = on_str == "on",
    };
    return stream;
}

using BoxSet = boost::container::small_vector<Box, 18>;
template <typename VectorT>
struct VectorPush : public boost::icl::identity_based_inplace_combine<VectorT> {
    using type = VectorPush<VectorT>;
    void operator()(VectorT& vec, const VectorT& operand) const {
        vec.insert(vec.end(), operand.begin(), operand.end());
    }
};
using BoxMap = boost::icl::split_interval_map<s32, BoxSet, boost::icl::partial_absorber, std::less,
                                              VectorPush, boost::icl::inter_section, Box::Side>;

u64 Volume(const BoxSet& boxes, usize dimension = Box{}.bounds.size() - 1) {
    if (dimension == 0) {

        boost::icl::interval_set<int, std::less, Box::Side> core_sample;
        for (const auto& box : boxes) {
            if (box.on)
                core_sample += box.bounds[dimension];
            else
                core_sample -= box.bounds[dimension];
        }
        return boost::icl::length(core_sample);
    } else {
        BoxMap boxmap;
        for (const auto& box : boxes) boxmap += std::pair{box.bounds[dimension], BoxSet{box}};
        u64 volume = 0;
        for (const auto& [interval, set] : boxmap)
            volume += boost::icl::length(interval) * Volume(set, dimension - 1);
        return volume;
    }
}

void Run(const BoxSet& boxes) {
    auto start = std::chrono::steady_clock::now();
    u64 volume = Volume(boxes);
    auto end = std::chrono::steady_clock::now();
    fmt::print("{:3} boxes    volume: {:20}    time: {}\n", boxes.size(), volume,
               std::chrono::duration<double, std::milli>{end - start});
}

int main() {
    std::ifstream input_file{"input.txt"};
    BoxSet boxes(std::istream_iterator<Box>{input_file}, {});
    Run(boxes);
    const Box max_bounds{.bounds{
        Box::Side{-50, 50},
        Box::Side{-50, 50},
        Box::Side{-50, 50},
    }};
    boost::remove_erase_if(boxes, [&max_bounds](const Box& box) { return !max_bounds.Contains(box); });
    Run(boxes);
}
