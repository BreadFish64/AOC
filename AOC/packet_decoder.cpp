#include "types.hpp"

class BitStream {
    const u8* ptr;
    usize pos = 0;
public:
    BitStream(const u8* ptr) : ptr{ptr} {}
    BitStream(const BitStream&) = delete;
    usize GetPos() const { return pos; }
    u64 Extract(unsigned bits) {
        u64 bin;
        const u64 cpos = pos;
        std::memcpy(&bin, ptr + cpos / CHAR_BIT, sizeof(bin));
        bin >>= cpos % CHAR_BIT;
        pos = cpos + bits;
        const u64 mask = (1ull << bits) - 1ull;
        bin &= mask;
        bin = __builtin_bitreverse64(bin);
        bin >>= 64 - bits;
        return bin;
    };
};

class Packet {
    std::vector<Packet> sub_packets;
    u64 literal = 0;
    std::optional<u64> cached_val{};
    u8 version;
    u8 type_id;

    u64 ComputeVal() {
        switch (type_id) {
        case 0: {
            u64 val = 0;
            for (auto& p : sub_packets) val += p();
            return val;
        }
        case 1: {
            u64 val = 1;
            for (auto& p : sub_packets) val *= p();
            return val;
        }
        case 2: return std::ranges::min(sub_packets, std::less{}, &Packet::operator())();
        case 3: return std::ranges::max(sub_packets, std::less{}, &Packet::operator())();
        case 4: return literal;
        case 5: return sub_packets[0]() > sub_packets[1]();
        case 6: return sub_packets[0]() < sub_packets[1]();
        case 7: return sub_packets[0]() == sub_packets[1]();
        default: throw 0;
        }
    }

public:
    Packet(BitStream& stream) {
        version = stream.Extract(3);
        type_id = stream.Extract(3);
        if (type_id == 4) {
            bool more = true;
            while (more) {
                more = stream.Extract(1);
                literal <<= 4;
                literal |= stream.Extract(4);
            }
        } else {
            unsigned length_type_id = stream.Extract(1);
            if (length_type_id == 0) {
                const u64 sub_packet_bits = stream.Extract(15);
                const u64 end = stream.GetPos() + sub_packet_bits;
                while (stream.GetPos() < end) sub_packets.emplace_back(stream);
            } else {
                u64 sub_packets_count = stream.Extract(11);
                while (sub_packets_count--) sub_packets.emplace_back(stream);
            }
        }
    }
    u64 VersionSum() const {
        u64 version_sum = version;
        for (const auto& p : sub_packets) version_sum += p.VersionSum();
        return version_sum;
    }
    u64 operator()() {
        if (cached_val) return *cached_val;
        cached_val = ComputeVal();
        return *cached_val;
    }
};

constexpr u8 HexDigit(char c) {
    int num = c - '0';
    int alpha = c - 'A';
    u8 digit = alpha < 0 ? num : alpha + 0xA;
    digit = __builtin_bitreverse8(digit) >> 4;
    return digit;
}

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector hex(std::istreambuf_iterator<char>{input_file}, {});
    std::vector<u8> bin(hex.size() / 2);
    auto hex_iter = hex.begin();
    for (u8& byte : bin) byte = HexDigit(*hex_iter++) | HexDigit(*hex_iter++) << 4;

    BitStream stream{bin.data()};
    Packet packet{stream};
    fmt::print("version sum: {}\n", packet.VersionSum());
    fmt::print("packet eval: {}\n", packet());
}
