#include <catch2/catch_test_macros.hpp>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

std::vector<int> parse(const std::string& input_file) {
  std::ifstream in(input_file);
  if (!in.good()) {
    throw std::runtime_error("couldn't open input file");
  }
  return std::vector<int> {
    std::istream_iterator<int>(in),
    std::istream_iterator<int>()
  };
}

std::pair<int, int> two_sum(const std::vector<int>& input, const int target) {
  for (const auto i : input) {
    for (const auto j : input) {
      if (i + j == target) return {i, j};
    }
  }
  return {0, 0};
}

TEST_CASE() {
  std::vector<int> input = parse("src/day-01.input");
  std::pair<int, int> numbers = two_sum(input, 2020);
  REQUIRE(numbers.first * numbers.second == 787776);
}
