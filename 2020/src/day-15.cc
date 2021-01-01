#include <catch2/catch_test_macros.hpp>
#include <ranges>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "lib.h"

auto parse(const std::string& input) {
  std::vector<int> out;
  std::stringstream ss {input};
  for (int i; ss >> i;) {
    out.push_back(i);
    if (ss.peek() == ',') ss.ignore();
  }
  return out;
}

auto solve_part_one(const auto& input, int turns) {
  std::unordered_map<int, std::vector<int>> numbers;
  for (int i = 0; i < input.size(); i++) {
    numbers[input[i]] = {i + 1};
  }
  int last_number_spoken = input.back();
  for (auto i = input.size() + 1; i <= turns; i++) {
    if (numbers.contains(last_number_spoken)) {
      auto& turns = numbers[last_number_spoken];
      if (turns.size() == 1) {
	last_number_spoken = 0;
      } else {
	last_number_spoken = turns[turns.size() - 1] - turns[turns.size() - 2];
      }
    } else {
      last_number_spoken = 0;    
    }
    numbers[last_number_spoken].push_back(i);
  }
  return last_number_spoken;
}

TEST_CASE("day fifteen") {
  SECTION("example") {
    auto input = parse("0,3,6");
    REQUIRE(436 == solve_part_one(input, 2020));
  }
  SECTION("part one") {
    auto input = parse("20,0,1,11,6,3");
    REQUIRE(421 == solve_part_one(input, 2020));
  }
  SECTION("part two") {
    auto input = parse("20,0,1,11,6,3");
    REQUIRE(436 == solve_part_one(input, 30'000'000));
  }
}
