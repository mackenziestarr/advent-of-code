#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include "lib.h"

constexpr auto parse = [](auto&& in) {
  return aoc::parse(in, [](const std::string& line) {
    return std::atoi(line.c_str());
  });
};

auto adjacent_difference_distribution(auto v) {
  std::ranges::sort(v);
  std::adjacent_difference(v.begin(), v.end(), v.begin());
  std::unordered_map<int, std::size_t> out;
  for (const auto& i : v) out[i] += 1;
  return out;
}

TEST_CASE("day 10") {
  SECTION("example case") {
    auto input = parse(std::istringstream {
      R"(16
         10
         15
          5
          1
         11
          7
         19
          6
         12
          4)"
    });
    auto dist = adjacent_difference_distribution(input);
    REQUIRE((dist[3] + 1) * dist[1] == 35);
  }
  SECTION("part one") {
    auto input = parse(std::ifstream{"input/day-10.input"});
    auto dist = adjacent_difference_distribution(input);
    REQUIRE((dist[3] + 1) * dist[1] == 2210);
  }
}
