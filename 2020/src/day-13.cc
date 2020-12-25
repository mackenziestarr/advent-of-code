#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <ranges>
#include "lib.h"

std::pair<int, std::vector<int>> parse(auto&& istream) {
  std::string line;
  std::getline(istream, line);
  int target = std::atoi(line.c_str());
  std::getline(istream, line);
  std::stringstream ss{line};
  std::string number;
  std::vector<int> out;
  while(std::getline(ss, number, ',')) {
    if (number != "x")
      out.push_back(std::atoi(number.c_str()));
  }
  return {target, out};
}

auto get_earliest_bus(const auto& ids, const auto target) {
  auto pairs =
    ids | std::views::transform([&target](const auto i) -> std::pair<int, int> {
      return {i, (target / i + 1) * i};
    });
  return *std::ranges::min_element(pairs,
    [&target](const auto& lhs, const auto& rhs) {
      return lhs.second < rhs.second;
    }
  );
}

TEST_CASE("day thirteen") {
  SECTION("example") {
    const auto& [target, ids] = parse(std::istringstream{
	"939\n7,13,x,x,59,x,31,19\n"
    });
    const auto& [id, ts] = get_earliest_bus(ids, target);
    REQUIRE((ts - target) * id == 295);
  }
  SECTION("part one") {
    const auto& [target, ids] = parse(std::ifstream{"input/day-13.input"});
    const auto& [id, ts] = get_earliest_bus(ids, target);
    REQUIRE((ts - target) * id == 295);
  }
}
