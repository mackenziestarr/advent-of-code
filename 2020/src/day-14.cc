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
#include <unordered_map>

uint64_t apply_bitmask(const std::string& bitmask, uint64_t input) {
  std::bitset<sizeof(uint64_t)*8> bitset {input};
  for (std::size_t i = 0; i < bitmask.size(); i++) {
    if (bitmask[i] == 'X') continue;
    bitset[bitmask.size() - 1 - i] = bitmask[i] == '1';
  }
  return bitset.to_ullong();
}

auto decode_memory(const std::string& bitmask, uint64_t input) {
  std::bitset<36> bitset {input};
  std::string bitset_str = bitset.to_string();
  std::vector<std::size_t> floating_positions;
  for (std::size_t i = 0; i < bitmask.size(); i++) {
    if (bitmask[i] == 'X') {
      bitset_str[i] = 'X';
      floating_positions.push_back(i);
    }
    else if (bitmask[i] == '1') {
      bitset_str[i] = '1';
    }
  }

  std::vector<uint64_t> out;
  std::size_t size = std::pow(2, floating_positions.size());
  for (std::size_t i = 0; i < size; i++) {
    std::string address {bitset_str};
    std::bitset<36> b {i};
    for (std::size_t i = 0; i < floating_positions.size(); i++) {
      address[floating_positions[i]] = b[floating_positions.size() - 1 - i] ? '1' : '0';
    }
    std::bitset<36> el(address);
    out.push_back(el.to_ullong());
  }

  return out;
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
    std::unordered_map<uint64_t,uint64_t> address_space;

    for (const auto& [bitmask, pairs] : input) {
      for (const auto& [address, value] : pairs) {
        address_space[address] = apply_bitmask(bitmask, value);
      }
    }

    return std::accumulate(
      address_space.begin(),
      address_space.end(),
      uint64_t{0},
      [](auto sum, const auto& pair) {
	const auto& [key, value] = pair;
	return sum += value;
      }
    );
}

uint64_t solve_part_two(const auto& input) {
    std::unordered_map<uint64_t,uint64_t> address_space;

    for (const auto& [bitmask, pairs] : input) {
      for (const auto& [base_address, value] : pairs) {
	const auto& addresses = decode_memory(bitmask, base_address);
	for (const auto& address : addresses) {
	  address_space[address] = value;
	}
      }
    }

    return std::accumulate(
      address_space.begin(),
      address_space.end(),
      uint64_t{0},
      [](auto sum, const auto& pair) {
	const auto& [key, value] = pair;
	return sum += value;
      }
    );
}

TEST_CASE("day fourteen") {
  SECTION("apply_bitmask") {
    REQUIRE(apply_bitmask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
			 0b000000000000000000000000000000001011)
                      == 0b000000000000000000000000000001001001);
    REQUIRE(apply_bitmask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
			 0b000000000000000000000000000001100101)
	             ==  0b000000000000000000000000000001100101);
    REQUIRE(apply_bitmask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
			 0b000000000000000000000000000000000000)
                      == 0b000000000000000000000000000001000000);
  }
  SECTION("decode_memory") {
    const std::string mask { "00000000000000000000000000000000X0XX" };
    const uint64_t input  { 0b000000000000000000000000000000011010 };
    const std::vector<uint64_t> expected {
      0b000000000000000000000000000000010000,
      0b000000000000000000000000000000010001,
      0b000000000000000000000000000000010010,
      0b000000000000000000000000000000010011,
      0b000000000000000000000000000000011000,
      0b000000000000000000000000000000011001,
      0b000000000000000000000000000000011010,
      0b000000000000000000000000000000011011
    };
    REQUIRE(decode_memory(mask, input) == expected);
  }
  SECTION("part one example input") {
    auto input = parse(std::stringstream{
R"(mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
mem[8] = 11
mem[7] = 101
mem[8] = 0)"
      });
    uint64_t result = solve_part_one(input);
    REQUIRE(result == 165);
    
  }
  SECTION("part two example input") {
    auto input = parse(std::stringstream{
R"(mask = 000000000000000000000000000000X1001X
mem[42] = 100
mask = 00000000000000000000000000000000X0XX
mem[26] = 1)"	
      });
    uint64_t result = solve_part_two(input);
    REQUIRE(result == 208);
  }
  SECTION("part one") {
    auto input = parse(std::ifstream { "input/day-14.input" });
    REQUIRE(solve_part_one(input) == 14839536808842);
  }
  SECTION("part two") {
    auto input = parse(std::ifstream { "input/day-14.input" });
    REQUIRE(solve_part_two(input) == 4215284199669);
  }  
}
