#include <catch2/catch_test_macros.hpp>
#include <utility>
#include <fstream>
#include <numeric>
#include <iostream>

int decode(const std::string_view& input) {
  std::string_view row_tokens = input.substr(0, 7);
  std::string_view col_tokens = input.substr(7);

  int low = 0, high = 127, mid;
  for (const auto& t : row_tokens) {
    mid = (high + low) / 2;
    if (t == 'F') high = mid;
    if (t == 'B') low = mid + 1;
  }
  int row = low;

  low = 0, high = 7;
  for (const auto& t : col_tokens) {
    mid = (high + low) / 2;
    if (t == 'L') high = mid;
    if (t == 'R') low = mid + 1;
  }
  int col = low;

  return (row * 8) + col;
}

TEST_CASE("part one") {
  std::ifstream in("input/day-05.input");
  if (!in) throw std::runtime_error("couldn't open file");

  int answer =
    std::transform_reduce(
      (std::istream_iterator<std::string>(in)),
      std::istream_iterator<std::string>(),
      int{0},
      [](int lhs, int rhs) { return std::max(lhs, rhs); },
      [](const std::string_view& s) { return decode(s); });
  
  REQUIRE(answer == 998);
}
