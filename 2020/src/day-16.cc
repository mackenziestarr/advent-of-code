#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <ranges>
#include "lib.h"

struct InclusiveRange {
  int lower;
  int upper;
  bool is_in_range(const int x) const {
    return lower <= x && x <= upper;
  }
};

struct Rule {
  std::string name;
  InclusiveRange lower;
  InclusiveRange upper;
  bool is_in_ranges(const int x) const {
    return lower.is_in_range(x)
        || upper.is_in_range(x);
  }
};

Rule parse_rule(const std::string& input) {
  std::regex re{ "([^\\:]+)\\: (\\d+)-(\\d+) or (\\d+)-(\\d+)" };
  std::smatch m;
  std::regex_match(input, m, re);
  return {
    m[1].str(),
    { std::stoi(m[2].str()), std::stoi(m[3].str()) },
    { std::stoi(m[4].str()), std::stoi(m[5].str()) },
  };
}

auto parse(auto&& in) {
  std::vector<Rule> rules;
  std::vector<int> your_ticket;
  std::vector<std::vector<int>> nearby_tickets;

  std::string line;
  while(std::getline(in, line)) {
    if (line.empty()) break;    
    rules.push_back(parse_rule(line));
  }

  for (char c = in.peek(); !std::isdigit(c); c = in.peek()) in.ignore();
  std::getline(in, line);
  your_ticket = aoc::split(line, ',');


  for (char c = in.peek(); !std::isdigit(c); c = in.peek()) in.ignore();
  while (std::getline(in, line)) {
    const auto& vec = aoc::split(line, ',');
    nearby_tickets.push_back(vec);
  }
  
  return std::make_tuple(rules, your_ticket, nearby_tickets);
}

std::stringstream example_input {
R"(class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12)"
};


auto solve_part_one(const auto& nearby_tickets, const auto& rules) {
  int sum = 0;
  auto is_valid = [&](const auto& value) -> bool {
    return std::any_of(rules.begin(), rules.end(), [&value](const auto& rule) {
      return rule.is_in_ranges(value);
    });
  };
  int i = 0;
  for (const auto& nearby_ticket : nearby_tickets) {
    for (const auto& ticket_number : nearby_ticket) {
      if (!is_valid(ticket_number)) sum += ticket_number;
    }
  }
  return sum;
}

TEST_CASE("day sixteen") {
  SECTION("example case") {
    const auto& [rules, your_ticket, nearby_tickets] = parse(example_input);
    REQUIRE(solve_part_one(nearby_tickets, rules) == 71);
  }
  SECTION("part one") {
    const auto& [rules, your_ticket, nearby_tickets] = parse(std::ifstream {"input/day-16.input"});
    REQUIRE(solve_part_one(nearby_tickets, rules) == 26980);
  }
}
