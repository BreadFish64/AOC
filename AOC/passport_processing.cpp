#include <algorithm>
#include <fstream>
#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string>

#include <fmt/ostream.h>

#include "types.hpp"

class Passport {
    std::map<std::string, std::string> fields;

    friend std::istream& operator>>(std::istream& stream, Passport& passport) {
        std::string str;
        passport.fields.clear();
        while (std::getline(stream, str)) {
            if (str.size() <= 1) return stream;
            std::stringstream line_stream{str};
            while (std::getline(line_stream, str, ':')) {
                std::string value;
                std::getline(line_stream, value, ' ');
                passport.fields.emplace(std::move(str), std::move(value));
            }
        };
        if (auto it = passport.fields.find("pid"); it->second.starts_with('#'))
            throw std::exception();
        return stream;
    }

    static const inline std::map<std::string, std::string> required{
        {"byr", ""}, {"iyr", ""}, {"eyr", ""}, {"hgt", ""}, {"hcl", ""}, {"ecl", ""}, {"pid", ""}};
    static const inline std::set<std::string> eye_colors{"amb", "blu", "brn", "gry",
                                                         "grn", "hzl", "oth"};

public:
    bool Valid1() const {
        return std::ranges::includes(fields, required, [](const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
        });
    }

    bool Valid2() const {
        try {
            auto byr = std::stoul(fields.at("byr"));
            if (byr < 1920 || byr > 2002) return false;
            auto iyr = std::stoul(fields.at("iyr"));
            if (iyr < 2010 || iyr > 2020) return false;
            auto eyr = std::stoul(fields.at("eyr"));
            if (eyr < 2020 || eyr > 2030) return false;
            const auto& hgt_str = fields.at("hgt");
            auto hgt = std::stoul(hgt_str);
            if (hgt_str.ends_with("cm")) {
                if (hgt < 150 || hgt > 193) return false;
            } else if (hgt_str.ends_with("in")) {
                if (hgt < 59 || hgt > 76) return false;
            } else {
                return false;
            }
            const auto& hcl_str = fields.at("hcl");
            if (hcl_str[0] != '#') return false;
            std::size_t hcl_count{};
            auto hcl = std::stoul(hcl_str.substr(1), &hcl_count, 16);
            if (hcl_count != 6) return false;
            if (!eye_colors.count(fields.at("ecl"))) return false;
            std::size_t pid_count{};
            auto pid = std::stoull(fields.at("pid"), &pid_count);
            if (pid_count != 9) return false;
        } catch (std::exception e) { return false; }
        return true;
    }
};

int main() {
    std::ifstream input_file{"input.txt"};
    std::vector<Passport> passports{std::istream_iterator<Passport>{input_file}, {}};
    u32 valid = std::ranges::count_if(passports,
                                      [](const Passport& passport) { return passport.Valid1(); });
    fmt::print("{} valid passports for part one\n", valid);
    valid = std::ranges::count_if(passports,
                                  [](const Passport& passport) { return passport.Valid2(); });
    fmt::print("{} valid passports for part two\n", valid);
}