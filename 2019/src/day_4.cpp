#include <stack>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

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

namespace part_one {
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
}
namespace part_two {
  bool is_valid_password(int password) {
    std::stack<int> digits = to_digits(password);
    std::map<int, int> map;
    if (digits.size() != 6) return false;
    int previous = -1;
    while (!digits.empty()) {
      int digit = digits.top();
      if (previous > digit) return false;
      previous = digit;
      map[digit]++;
      digits.pop();
    }
    for (auto i = map.begin(); i != map.end(); i++) {
      if (i->second == 2) {
        return true;
      }
    }
    return false;
  }
}


#ifdef RUN_TESTS
#define CATCH_CONFIG_MAIN
#include "../third_party/catch.hpp"
TEST_CASE("evaluates part one password criteria", "[part_one::is_valid_password]") {
  REQUIRE(part_one::is_valid_password(111111) == true);
  REQUIRE(part_one::is_valid_password(223450) == false);
  REQUIRE(part_one::is_valid_password(123789) == false);
}
TEST_CASE("evaluates part two password criteria", "[part_two::is_valid_password]") {
  REQUIRE(part_two::is_valid_password(112233) == true);
  REQUIRE(part_two::is_valid_password(123444) == false);
  REQUIRE(part_two::is_valid_password(111122) == true);
}
#else
int main() {
  std::cout << "advent of code 2019: day 4" << '\n';
  constexpr int range_start = 246540;
  constexpr int range_end = 787419;
  int part_one_count = 0;
  int part_two_count = 0;
  for (int i = range_start; i <= range_end; i++) {
    if (part_one::is_valid_password(i)) part_one_count++;
    if (part_two::is_valid_password(i)) part_two_count++;
  }
  std::cout << "part one:\t" << part_one_count << '\n';
  std::cout << "part two:\t" << part_two_count << '\n';

  return 0;
}
#endif
