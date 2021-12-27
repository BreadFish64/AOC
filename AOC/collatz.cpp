#include "types.hpp"

#include <chrono>
#include <future>

using u128 = __uint128_t;
auto ctz(u128 x) {
    u64 lo = static_cast<u64>(x);
    u64 hi = static_cast<u64>(x >> 64);
    return lo ? std::countr_zero(lo) : (std::countr_zero(hi) + 64);
}
constexpr u128 max = u128{1} << 126;
constexpr u64 pulse = u128{1} << 28;
int main() {
    auto start = std::chrono::steady_clock::now();
    std::atomic<u64> chunk = 1;
    std::vector<std::future<void>> futures;
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        futures.emplace_back(std::async(std::launch::async, [&chunk, start, thread = i] {
            for (;;) {
                auto time = std::chrono::duration<double>(std::chrono::steady_clock::now() - start);
                const u64 chunk_start = chunk.fetch_add(pulse);
                const u64 chunk_end = chunk_start + pulse;
                fmt::print("thread {}: {} @ {}\n", thread, chunk_start, time);
                for (u64 val = chunk_start; val < chunk_end; val += 2) {
                    u128 n = val;
                    do {
                        n = n * 3 + 1;
                        auto e2 = ctz(n);
                        n >>= e2;
                        if (n > max) [[unlikely]] {
                            fmt::print("potential overflow: {} -> {:016X}{:016X}\n", val,
                                       static_cast<u64>(n >> 64), static_cast<u64>(n));
                            return;
                        }
                    } while (n > val);
                    if (n == val) [[unlikely]] {
                        fmt::print("loop point: {}\n", val);
                        return;
                    }
                }
            }
        }));
    }
    for (auto& future : futures) future.wait();
    auto time = std::chrono::duration<double>(std::chrono::steady_clock::now() - start);
    fmt::print("{}\n", time);
}
