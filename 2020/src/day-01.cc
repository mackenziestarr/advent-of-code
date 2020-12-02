#include <catch2/catch_test_macros.hpp>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <numeric>

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

void combine(std::vector<int>& input, int offset, int k, std::vector<int>& combo, std::vector<std::vector<int>>& out) {
  if (k == 0) {
    out.push_back(combo);
    return;
  }
  for (int i = offset; i <= input.size() - k; i++) {
    combo.push_back(input[i]);
    combine(input, i + 1, k - 1, combo, out);
    combo.pop_back();
  }
}

std::vector<std::vector<int>> combine(std::vector<int>& input, int k) {
  std::vector<std::vector<int>> out;
  std::vector<int> combo;  
  combine(input, 0, k, combo, out);
  return out;
}

std::vector<int> combination_summing_to_n(std::vector<int>& input, const int k, const int n) {
  std::vector<std::vector<int>> combinations = combine(input, k);
  for (const auto& c : combinations) {
    if (std::accumulate(c.begin(), c.end(), 0) == n) return c;
  }
  return {};
}

TEST_CASE("part one") {
  std::vector<int> input = parse("src/day-01.input");
  std::vector<int> numbers = combination_summing_to_n(input, 2, 2020);
  int answer = std::accumulate(numbers.begin(), numbers.end(), 1, std::multiplies<int>());
  REQUIRE(answer == 787776);
}

TEST_CASE("part two") {
  std::vector<int> input = parse("src/day-01.input");
  std::vector<int> numbers = combination_summing_to_n(input, 3, 2020);
  int answer = std::accumulate(numbers.begin(), numbers.end(), 1, std::multiplies<int>());
  REQUIRE(answer == 262738554);
}

