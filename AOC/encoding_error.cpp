#include <deque>
#include <fstream>
#include <ranges>
#include <set>
#include <vector>

#include <fmt/ostream.h>

#include "types.hpp"

namespace ranges {
using namespace std::ranges;
}

template <ranges::bidirectional_range R>
auto TwoSum(const R& data, const ranges::range_reference_t<R> target) noexcept {
    auto front = data.begin();
    auto rear = --data.end();
    while (front != rear) {
        auto sum = *front + *rear;
        if (sum < target) {
            ++front;
        } else if (sum > target) {
            --rear;
        } else {
            break;
        }
    }
    return (*front + *rear == target) ? std::make_pair(front, rear)
                                      : std::make_pair(data.end(), data.end());
}

template <ranges::forward_range R>
auto CrackKey(const R& numbers) {
    std::multiset<ranges::range_value_t<R>> buffer;
    std::deque<typename decltype(buffer)::const_iterator> remove_queue;
    auto it = numbers.begin();
    for (unsigned i{0}; i != 25; ++i) remove_queue.emplace_back(buffer.emplace(*it++));
    while (TwoSum(buffer, *it).first != buffer.end()) {
        buffer.erase(remove_queue.front());
        remove_queue.pop_front();
        remove_queue.emplace_back(buffer.emplace(*it++));
    }
    return *it;
}

template <ranges::forward_range R>
auto ContiguousSum(const R& data, const ranges::range_reference_t<R> target) noexcept {
    auto begin = data.begin();
    auto end = ++data.begin();
    auto rolling_sum = *begin + *end;
    while (rolling_sum != target) {
        if (rolling_sum > target) {
            rolling_sum -= *begin;
            ++begin;
        } else {
            ++end;
            rolling_sum += *end;
        }
    }
    return ranges::subrange(begin, end);
}

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<u64> numbers{std::istream_iterator<u64>{input_file}, {}};
    u64 key = CrackKey(numbers);
    fmt::print("Invalid number\t{}\n", key);
    auto [min, max] = ranges::minmax_element(ContiguousSum(numbers, key));
    fmt::print("Sum min max\t{}\n", *min + *max);
}
