#include <catch2/catch_test_macros.hpp>
#include <numeric>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_set>
#include <iostream>
#include <utility>
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

std::optional<int> find_error(const std::vector<long>& input, std::size_t preamble_length) {
  for (std::size_t i = 0; i < input.size() - preamble_length; i++) {
    auto summed_pairs = sum_pairs(
      std::vector<long>{input.begin() + i, input.begin() + i + preamble_length}
    );
    if (!summed_pairs.contains(input[i + preamble_length])) {
      return input[i + preamble_length];
    }
  }
  return {};
}

std::pair<int, int> find_range(const std::vector<long>& input, long target) {
  for (std::size_t start = 0; start < input.size(); start++) {
    long sum = input[start], min = sum, max = sum;
    for (std::size_t end = start + 1; end < input.size(); end++) {
      min = std::min(min, input[end]);
      max = std::max(max, input[end]);
      sum += input[end];
      if (sum > target) break;
      if (sum == target) return {min, max};
    }
  }
  return {};
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
    const auto& [min, max] = find_range(input, 127);
    REQUIRE(min + max == 62);
  }
  SECTION("part one") {
    const auto input = parse(std::ifstream{"input/day-09.input"});
    REQUIRE(find_error(input, 25) == 373803594);
  }
  SECTION("part two") {
    const auto input = parse(std::ifstream{"input/day-09.input"});
    const auto& [min, max] = find_range(input, 373803594);
    REQUIRE(min + max == 51152360);
  }
}
