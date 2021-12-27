#include "types.hpp"

#include <future>

constexpr bool part_two = true;

template <std::unsigned_integral U>
constexpr U Pow(U base, U exponent) {
    U result = 1;
    U power = base;
    while (exponent) {
        if (exponent & 1) result *= power;
        power *= power;
        exponent >>= 1;
    }
    return result;
}

constexpr void add(int& dst, const int& src) { dst += src; }
constexpr void mul(int& dst, const int& src) { dst *= src; }
constexpr void _div(int& dst, const int& src) {
    if (src == 0) __builtin_unreachable();
    dst /= src;
}
constexpr void mod(int& dst, const int& src) {
    if (dst < 0 || src <= 0) __builtin_unreachable();
    dst %= src;
}
constexpr void eql(int& dst, const int& src) { dst = dst == src; }

template <unsigned digit>
s64 MONAD(int w, int z) {}

template <unsigned digit>
[[gnu::flatten, gnu::target("avx2")]] s64 DISPATCH(int z) {
    constexpr u64 multiplier = Pow(10ull, 13ull - digit);
    if (!part_two) {
        for (int w = 9; w > 0; --w) {
            if (s64 val = MONAD<digit>(w, z); val >= 0) return val + w * multiplier;
        }
    } else {
        for (int w = 1; w < 10; ++w) {
            if (s64 val = MONAD<digit>(w, z); val >= 0) return val + w * multiplier;
        }
    }
    return -1;
}

template <>
s64 MONAD<13>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 26);
    add(x, -12);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 14);
    mul(y, x);
    add(z, y);

    return z == 0 ? 0 : -1;
}

template <>
s64 MONAD<12>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 26);
    add(x, -6);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 6);
    mul(y, x);
    add(z, y);

    return DISPATCH<13>(z);
}

template <>
s64 MONAD<11>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 26);
    add(x, -5);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 14);
    mul(y, x);
    add(z, y);

    return DISPATCH<12>(z);
}

template <>
s64 MONAD<10>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 26);
    add(x, 0);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 2);
    mul(y, x);
    add(z, y);

    return DISPATCH<11>(z);
}

template <>
s64 MONAD<9>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 1);
    add(x, 11);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 1);
    mul(y, x);
    add(z, y);

    return DISPATCH<10>(z);
}

template <>
s64 MONAD<8>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 26);
    add(x, -9);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 10);
    mul(y, x);
    add(z, y);

    return DISPATCH<9>(z);
}

template <>
s64 MONAD<7>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 26);
    add(x, -8);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 13);
    mul(y, x);
    add(z, y);

    return DISPATCH<8>(z);
}

template <>
s64 MONAD<6>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 1);
    add(x, 12);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 13);
    mul(y, x);
    add(z, y);

    return DISPATCH<7>(z);
}

template <>
s64 MONAD<5>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 1);
    add(x, 12);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 4);
    mul(y, x);
    add(z, y);

    return DISPATCH<6>(z);
}

template <>
s64 MONAD<4>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 1);
    add(x, 12);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 11);
    mul(y, x);
    add(z, y);

    return DISPATCH<5>(z);
}

thread_local int digit_two{};

template <>
s64 MONAD<3>(int w, int z) {
    fmt::print("pulse {:3}{}\n", digit_two, w);
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 26);
    add(x, 0);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 7);
    mul(y, x);
    add(z, y);

    return DISPATCH<4>(z);
}

template <>
s64 MONAD<2>(int w, int z) {
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 1);
    add(x, 10);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 2);
    mul(y, x);
    add(z, y);

    return DISPATCH<3>(z);
}

template <>
[[gnu::flatten, gnu::target("avx2")]] s64 DISPATCH<2>(int z) {
    std::array<std::future<s64>, 8> futures;
    constexpr auto iteration = [](int w, int z) -> s64 {
        digit_two = w;
        constexpr u64 multiplier = Pow(10ull, 13ull - 2);
        if (s64 val = MONAD<2>(w, z); val >= 0) return val + w * multiplier;
        return -1;
    };
    if (!part_two) {
        for (int w = 9; w > 0; --w) {
            futures[9 - w] = std::async(std::launch::async, iteration, w, z);
        }
    } else {
        for (int w = 1; w < 10; ++w) {
            futures[w - 1] = std::async(std::launch::async, iteration, w, z);
        }
    }
    for (auto& future : futures) future.wait();
    for (auto& future : futures)
        if (s64 val = future.get(); val >= 0) return val;
    return -1;
}

template <>
s64 MONAD<1>(int w, int z) {
    {
        static auto prev_time = std::chrono::steady_clock::now();
        auto time = std::chrono::steady_clock::now();
        fmt::print("pulse {:2}    {}\n", w, std::chrono::duration<double>(time - prev_time));
    }
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 1);
    add(x, 14);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 13);
    mul(y, x);
    add(z, y);

    return DISPATCH<2>(z);
}

template <>
s64 MONAD<0>(int w, int z) {
    fmt::print("pulse {:1}\n", w);
    int x = 0, y = 0;
    mul(x, 0);
    add(x, z);
    mod(x, 26);
    _div(z, 1);
    add(x, 11);
    eql(x, w);
    eql(x, 0);
    mul(y, 0);
    add(y, 25);
    mul(y, x);
    add(y, 1);
    mul(z, y);
    mul(y, 0);
    add(y, w);
    add(y, 8);
    mul(y, x);
    add(z, y);

    return DISPATCH<1>(z);
}

int main() {
    s64 val = DISPATCH<0>(0);
    fmt::print("largest model number: {}\n", val);
}
