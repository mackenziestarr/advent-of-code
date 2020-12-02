#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

struct Policy {
  int lower_bound;
  int upper_bound;
  char letter;
};

struct Password {
  std::string text;
  Policy policy;
};

std::vector<Password> parse(const std::string& input_file) {
  std::ifstream in(input_file);
  std::vector<Password> out;
  if (!in.good()) {
    throw std::runtime_error("couldn't open input file");
  }
  std::string line;
  while(std::getline(in, line)) {
    std::istringstream ss(line);
    Password p;
    // example line from file "6-8 s: svsssszslpsp"
    ss >> p.policy.lower_bound;
    ss.ignore(std::numeric_limits<std::streamsize>::max(), '-');
    ss >> p.policy.upper_bound >> p.policy.letter;
    ss.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    ss >> p.text;
    out.emplace_back(p);
  }
  return out;
}

bool is_valid(const Password& p) {
  int count = std::count_if(
    p.text.cbegin(),
    p.text.cend(),
    [&p](char c) { return c == p.policy.letter; }
  );
  return p.policy.lower_bound <= count &&
         p.policy.upper_bound >= count;
}

TEST_CASE("part one") {
  std::vector<Password> passwords = parse("src/day-02.input");
  int answer = std::count_if(passwords.cbegin(), passwords.cend(), is_valid);
  REQUIRE(answer == 515);
}
