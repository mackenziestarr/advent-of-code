#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "lib.h"

struct Policy {
  int lower;
  int upper;
  char letter;
};

struct Password {
  std::string text;
  Policy policy;
};

namespace part_one {
  bool is_valid(const Password& p) {
    int count = std::count_if(
      p.text.cbegin(),
      p.text.cend(),
      [&p](char c) { return c == p.policy.letter; }
    );
    return p.policy.lower <= count &&
           p.policy.upper >= count;
  }
}

namespace part_two {
  bool is_valid(const Password& p) {
    return (p.text[p.policy.lower - 1] == p.policy.letter) !=
           (p.text[p.policy.upper - 1] == p.policy.letter);
  }
}

TEST_CASE("day two") {
  std::vector<Password> passwords = aoc::parse(
    std::ifstream{"input/day-02.input"},
    [](const std::string& line) -> Password {
      std::istringstream ss(line);
      Password p;
      // example line from file "6-8 s: svsssszslpsp"
      ss >> p.policy.lower;
      ss.ignore(std::numeric_limits<std::streamsize>::max(), '-');
      ss >> p.policy.upper >> p.policy.letter;
      ss.ignore(std::numeric_limits<std::streamsize>::max(), ':');
      ss >> p.text;
      return p;
    }
  );
  SECTION("part one") {
    int answer = std::count_if(passwords.cbegin(), passwords.cend(), part_one::is_valid);
    REQUIRE(answer == 515);
  }
  SECTION("part two") {
    int answer = std::count_if(passwords.cbegin(), passwords.cend(), part_two::is_valid);
    REQUIRE(answer == 711);
  }
}
