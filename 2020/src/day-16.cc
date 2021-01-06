#define CATCH_CONFIG_ENABLE_PAIR_STRINGMAKER
#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <ranges>
#include <unordered_map>
#include <set>
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

std::stringstream example_input_1 {
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

std::stringstream example_input_2 {
R"(class: 0-1 or 4-19
row: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9)"
};

auto is_valid_for_rules(const auto& rules) {
  return [&rules](const auto& value) -> bool {
    return std::any_of(rules.begin(), rules.end(), [&value](const auto& rule) {
      return rule.is_in_ranges(value);
    });
  };
}

auto solve_part_one(const auto& nearby_tickets, const auto& rules) {
  int sum = 0;
  auto is_valid = is_valid_for_rules(rules);
  for (const auto& nearby_ticket : nearby_tickets) {
    for (const auto& ticket_number : nearby_ticket) {
      if (!is_valid(ticket_number)) sum += ticket_number;
    }
  }
  return sum;
}

auto solve_part_two(auto nearby_tickets, const auto& your_ticket, auto rules) {
  nearby_tickets.erase(
    std::remove_if(nearby_tickets.begin(), nearby_tickets.end(),
      [&rules](const auto& vec) {
	return !std::ranges::all_of(vec, is_valid_for_rules(rules));
    }),
    nearby_tickets.end());

  std::vector<std::pair<std::string, int>> fields;
  
  while (rules.size()) {
    for (std::size_t col = 0; col < nearby_tickets[0].size(); col++) {
      std::vector<int> column_values;
      for (std::size_t row = 0; row < nearby_tickets.size(); row++) {
	column_values.push_back(nearby_tickets[row][col]);
      }
      std::vector<std::size_t> matched_rule_indexes;
      for (std::size_t i = 0; i < rules.size(); i++) {
	auto rule = rules[i];
	const auto is_valid = [&rule](const auto& value) { return rule.is_in_ranges(value); };
	if (std::ranges::all_of(column_values, is_valid)) {
	  matched_rule_indexes.push_back(i);
	}
      }
      if (matched_rule_indexes.size() == 1) {
	auto rule = rules[matched_rule_indexes.front()];
	fields.push_back({rule.name, your_ticket[col]});
	rules.erase(rules.begin() + matched_rule_indexes.front());
      }
    }
  }
  return fields;
}

// TODO make stringstream shareable
TEST_CASE("day sixteen") {
  SECTION("example case #1") {
    const auto& [rules, your_ticket, nearby_tickets] = parse(example_input_1);
    REQUIRE(solve_part_one(nearby_tickets, rules) == 71);
  }
  SECTION("example case #2") {
    const auto& [rules, your_ticket, nearby_tickets] = parse(example_input_2);
    std::vector<std::pair<std::string, int>> fields {
      {"row",   11},      
      {"class", 12},
      {"seat",  13}
    };
    REQUIRE(solve_part_two(nearby_tickets, your_ticket, rules) == fields);
  }
  SECTION("part one") {
    const auto& [rules, your_ticket, nearby_tickets] = parse(std::ifstream {"input/day-16.input"});
    REQUIRE(solve_part_one(nearby_tickets, rules) == 26980);
  }
  SECTION("part two") {
    const auto& [rules, your_ticket, nearby_tickets] = parse(std::ifstream {"input/day-16.input"});
    auto fields = solve_part_two(nearby_tickets, your_ticket, rules);
    uint64_t product = 1;
    for (const auto& [k, v] : fields) {
      if (k.starts_with("departure")) {
	product *= v;
      }
    }
    REQUIRE(product == 3021381607403);
  }
}
