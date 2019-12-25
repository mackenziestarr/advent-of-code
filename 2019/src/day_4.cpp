#include <stack>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::stack<int> to_digits(int x) {
  std::stack<int> out;
  while(x != 0) {
    out.push(x % 10);
    x /= 10;
  }
  return out;
}

std::ostream &operator<<(std::ostream& os, const std::stack<int>& s) {
  std::stack copy = s;
  while (!copy.empty()) {
    os << copy.top();
    copy.pop();
    if (!copy.empty()) {
      os << ",";
    }
  }
  return os;
}

bool is_valid_password(int password) {
  std::stack<int> digits = to_digits(password);
  if (digits.size() != 6) return false;
  bool adjacent = false;
  int previous = -1;
  while (!digits.empty()) {
    int digit = digits.top();
    if (previous > digit) return false;
    if (previous == digit) adjacent = true;
    previous = digit;
    digits.pop();
  }
  return adjacent && true;
}

#ifdef RUN_TESTS
#define CATCH_CONFIG_MAIN
#include "../third_party/catch.hpp"
TEST_CASE("evaluates password criteria", "[]") {
  REQUIRE(is_valid_password(111111) == true);
  REQUIRE(is_valid_password(223450) == false);
  REQUIRE(is_valid_password(123789) == false);
}
#else
int main() {
  std::cout << "advent of code 2019: day 4" << '\n';
  constexpr int range_start = 246540;
  constexpr int range_end = 787419;
  int count = 0;
  for (int i = range_start; i <= range_end; i++) {
    if (is_valid_password(i)) {
      count++;
    }
  }
  std::cout << "part one:\t" << count << '\n';

  return 0;
}
#endif
