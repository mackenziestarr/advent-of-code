#include <catch2/catch_test_macros.hpp>
#include <numeric>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_set>
#include <iostream>
#include "lib.h"

auto sum_pairs(const std::vector<long>& input) {
  std::unordered_set<long> out;
  for (const auto& i : input) {
    for (const auto& j : input) {
      if (i != j) out.insert(i + j);
    }
  }
  return out;
}

int find_error(const std::vector<long>& input, std::size_t preamble_length) {
  for (std::size_t i = 0; i < input.size() - preamble_length; i++) {
    auto summed_pairs = sum_pairs(
      std::vector<long>{input.begin() + i, input.begin() + i + preamble_length}
    );
    if (!summed_pairs.contains(input[i + preamble_length])) {
      return input[i + preamble_length];
    }
  }
  return -1;
}

auto parse = [](auto&& in) {
  return aoc::parse(in, [](const std::string& line) -> long {
    return std::atol(line.c_str());
  });
};

TEST_CASE("day nine") {
  SECTION("example case") {
    const auto input = parse(std::istringstream {
      R"(35
         15
         25
         47
         40
         62
         55
         65
         95
        102
        117
        150
        182
        127
        219
        299
        277
        309
        576)"
      });
    REQUIRE(find_error(input, 5) == 127);
  }
  SECTION("part one") {
    const auto input = parse(std::ifstream{"input/day-09.input"});
    REQUIRE(find_error(input, 25) == 373803594);
  }
}
