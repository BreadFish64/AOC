#include <fstream>
#include <vector>
#include <fmt/ostream.h>
#include <immintrin.h>

constexpr unsigned CODE_WIDTH = 12;
constexpr unsigned SAFETY_MASK = (1 << CODE_WIDTH) - 1;
constexpr bool BTEST(unsigned n, unsigned pos) { return (n >> pos) & 1; }

std::uint16_t _mm256_movemask_epi16(__m256i vector) {
    auto packed_vector =
        _mm_packs_epi16(_mm256_extracti128_si256(vector, 0), _mm256_extracti128_si256(vector, 1));
    return _mm_movemask_epi8(packed_vector);
}

int main() {
    std::ifstream input_file{"input.txt", std::ios::binary};
    const std::vector<char> codes(std::istreambuf_iterator<char>{input_file}, {});
    const unsigned code_count = codes.size() / 13;
    std::vector<std::uint16_t> compact_codes(code_count);

    __m256i bit_sums{};
    for (std::size_t i = 0; i < code_count; ++i) {
        __m128i code =
            _mm_loadu_si128(reinterpret_cast<const __m128i*>(codes.data() + i * (CODE_WIDTH + 1)));
        code = _mm_sub_epi8(code, _mm_set1_epi8('0')); // convert to number
        // this is all optimized to one permutation but it's easier to think like this
        code =
            _mm_shuffle_epi8(code, _mm_set_epi8(0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
        compact_codes[i] =
            ~_mm_movemask_epi8(_mm_cmpeq_epi8(code, _mm_setzero_si128())) & SAFETY_MASK;

        auto expanded_code = _mm256_set_m128i(_mm_undefined_si128(), code);  // expand to AVX vector
        expanded_code = _mm256_permute4x64_epi64(expanded_code, 0b01010000); // prep for unpack
        expanded_code = _mm256_unpacklo_epi8(expanded_code, __m256i{});      // unpack to 16 bit int

        bit_sums = _mm256_add_epi16(bit_sums, expanded_code);
    }
    // create mask of bits that occur less than half the time and extract the mask into a number
    const auto common = _mm256_cmpgt_epi16(_mm256_set1_epi16(code_count / 2), bit_sums);
    const std::uint16_t epsilon = _mm256_movemask_epi16(common) & SAFETY_MASK;
    const std::uint16_t gamma = epsilon ^ SAFETY_MASK;
    const unsigned power = gamma * epsilon;
    fmt::print("gamma: {}\nepsilon: {}\npower: {}\n", gamma, epsilon, power);

    const auto EvalLifeSupport = [&](bool least_common) {
        std::vector<std::uint16_t> elimination_codes = compact_codes;
        for (int bit = CODE_WIDTH - 1; bit >= 0; --bit) {
            std::size_t ones = 0;
            for (std::uint16_t code : elimination_codes) ones += BTEST(code, bit);
            const bool common_bit_val = (ones >= (elimination_codes.size() - ones)) ^ least_common;

            std::size_t next_code_index = 0;
            for (std::uint16_t code : elimination_codes)
                if (BTEST(code, bit) == common_bit_val) elimination_codes[next_code_index++] = code;

            if (next_code_index == 0) return elimination_codes.back();
            elimination_codes.resize(next_code_index);
        }
        return elimination_codes.back();
    };
    const std::uint16_t O2 = EvalLifeSupport(false);
    const std::uint16_t CO2 = EvalLifeSupport(true);
    fmt::print("O2: {}\nCO2: {}\nrating: {}\n", O2, CO2, O2 * CO2);
}
