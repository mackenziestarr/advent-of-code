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
    return std::atol(line.c_str());
  });
};

auto adjacent_difference_distribution(auto v) {
  std::ranges::sort(v);
  std::adjacent_difference(v.begin(), v.end(), v.begin());
  std::unordered_map<int, std::size_t> out;
  for (const auto& i : v) out[i] += 1;
  return out;
}

long adapter_count(auto v) {
  v.push_back(0);
  std::ranges::sort(v);
  
  std::unordered_map<long, std::size_t> exists;
  for (std::size_t i = 0; i < v.size(); i++) exists[v[i]] = i;

  std::vector<long> ways(v.size(), 0);
  // there is only one way to get to the last element (n + 3)
  ways[v.size() - 1] = 1;

  for (int i = v.size() - 2; i >= 0; i--) {
    long sum = 0;  
    for (int j = 1; j <= 3; j++) {
      if (exists.contains(v[i] + j)) {
	sum += ways[exists[v[i] + j]];
      }
    }
    ways[i] = sum;
  }
  
  return ways.front();
}


TEST_CASE("day 10") {
  SECTION("example case #1") {
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
    auto count = adapter_count(input);
    REQUIRE(count == 8);
  }
  SECTION("example case #2") {
    auto input = parse(std::istringstream {
      R"(28
         33
         18
         42
         31
         14
         46
         20
         48
         47
         24
         23
         49
         45
         19
         38
         39
         11
         1
         32
         25
         35
         8
         17
         7
         9
         4
         2
         34
         10
         3)"
    });
    auto dist = adjacent_difference_distribution(input);
    REQUIRE((dist[3] + 1) * dist[1] == 220);
    auto count = adapter_count(input);
    REQUIRE(count == 19208);
  }
  SECTION("part one") {
    auto input = parse(std::ifstream{"input/day-10.input"});
    auto dist = adjacent_difference_distribution(input);
    REQUIRE((dist[3] + 1) * dist[1] == 2210);
  }
  SECTION("part two") {
    auto input = parse(std::ifstream{"input/day-10.input"});
    auto count = adapter_count(input);
    REQUIRE(count == 7086739046912);
  }
}
