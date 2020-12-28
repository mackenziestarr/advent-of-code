#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <ranges>
#include <cstdint>
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
    out.push_back(std::atoi(number.c_str()));
  }
  return {target, out};
}

auto get_earliest_bus(const auto& ids, const auto target) {
  constexpr auto zeros = [](const auto& i) { return i != 0; };

  const auto make_pairs = [&target](const auto bus_id) -> std::pair<int, int> {
    return {bus_id, (target / bus_id + 1) * bus_id};
  };

  auto pairs =
    ids
    | std::views::filter(zeros)
    | std::views::transform(make_pairs);
  
  return *std::ranges::min_element(pairs,
    [&target](const auto& lhs, const auto& rhs) {
      return lhs.second < rhs.second;
    }
  );
}

int64_t get_earliest_timestamp(const auto& ids) {
  int64_t min = 0, product = 1;
  for (int i = 0; i < ids.size(); i++) {
    if (ids[i] == 0) continue;
    while ((min + i) % ids[i] != 0) {
      min += product;
    }
    product *= ids[i];
  }
  return min;
}

TEST_CASE("day thirteen") {
  SECTION("example") {
    const auto& [target, ids] = parse(std::istringstream{
	"939\n7,13,x,x,59,x,31,19\n"
    });
    {
      const auto& [id, ts] = get_earliest_bus(ids, target);
      REQUIRE((ts - target) * id == 295);
    }
    {
      const auto ts = get_earliest_timestamp(ids);
      REQUIRE(ts == 1068781);
    }
    
  }
  SECTION("part one") {
    const auto& [target, ids] = parse(std::ifstream{"input/day-13.input"});
    const auto& [id, ts] = get_earliest_bus(ids, target);
    REQUIRE((ts - target) * id == 333);
  }
  SECTION("part two") {
    const auto& [_target, ids] = parse(std::ifstream{"input/day-13.input"});
    const auto& ts = get_earliest_timestamp(ids);
    REQUIRE(ts == 690123192779524);
  }
}
