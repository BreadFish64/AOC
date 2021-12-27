#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>

#include <immintrin.h>

#include <fmt/chrono.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

using namespace std::literals;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using f32 = float;
using f64 = double;

using usize = std::size_t;
using ssize = std::make_signed_t<usize>;

constexpr usize operator""_sz(unsigned long long x) { return static_cast<usize>(x); }

template <std::unsigned_integral I>
constexpr I AlignUp(I x, I alignment) {
    I mod = x % alignment;
    return mod ? x + alignment - mod : x;
}

template <std::unsigned_integral I>
constexpr I DivCeil(I x, I y) {
    return (x + y - 1) / y;
}