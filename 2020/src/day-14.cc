#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

uint64_t apply_bitmask(const std::string& bitmask, uint64_t input) {
  std::bitset<sizeof(uint64_t)*8> bitset {input};
  for (int i = 0; i < bitmask.size(); i++) {
    if (bitmask[i] == 'X') continue;
    bitset[bitmask.size() - 1 - i] = bitmask[i] == '1';
  }
  return bitset.to_ullong();
}

// TODO(@mstarr) this parser is haunted
auto parse(auto&& in) {
  std::pair<std::string, std::vector<std::pair<uint64_t,uint64_t>>> entry;
  std::vector<decltype(entry)> out;
  std::string line;
  while (std::getline(in, line)) {
    if (line.substr(0, 4) == "mask") {
      if (!entry.first.empty()) out.push_back(entry);
      entry.first = line.substr(7);
      entry.second = {};
    } else {
      const std::regex re { R"(^mem\[(\d+)\] = (\d+)$)" };
      std::smatch m;
      std::regex_match(line, m, re);
      entry.second.push_back({
	  std::stoull(m[1].str()),
	  std::stoull(m[2].str())
      });
    }
  }
  out.push_back(entry);
  return out;
}

uint64_t solve_part_one(const auto& input) {
    constexpr std::size_t size = std::pow(2, 20);
    std::vector<uint64_t> address_space(size, 0);


    for (const auto& [bitmask, pairs] : input) {
      for (const auto& [address, value] : pairs) {
        address_space[address] = apply_bitmask(bitmask, value);
      }
    }

    return std::accumulate(
      address_space.begin(),
      address_space.end(),
      uint64_t{0}
    );
}

TEST_CASE("day fourteen") {
  SECTION("helper functions") {
    const std::string mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X";
    REQUIRE(
	    apply_bitmask(mask, 0b000000000000000000000000000000001011)
	    == 0b000000000000000000000000000001001001);
    REQUIRE(
	    apply_bitmask(mask, 0b000000000000000000000000000001100101)
	    ==  0b000000000000000000000000000001100101);
    REQUIRE(
	    apply_bitmask(mask, 0b000000000000000000000000000000000000)
	    == 0b000000000000000000000000000001000000);
  }
  SECTION("example input") {
    auto input = parse(std::stringstream{
R"(mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
mem[8] = 11
mem[7] = 101
mem[8] = 0)"
      });
    uint64_t result = solve_part_one(input);
    REQUIRE(result == 165);
  }
  SECTION("part one") {
    auto input = parse(std::ifstream { "input/day-14.input" });
    REQUIRE(solve_part_one(input) == 14839536808842);
  }
}
