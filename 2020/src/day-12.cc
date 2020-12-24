#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <vector>
#include <sstream>
#include <fstream>
#include "lib.h"

struct Coordinate {
  int x, y;
  Coordinate& operator+=(const Coordinate& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
};

constexpr auto parse = [](auto&& in) {
  return aoc::parse(in, [](const std::string& line) -> std::pair<char, int> {
      return { line[0], std::atoi(line.substr(1).c_str()) };
  });
};

namespace part_one {
  auto navigate(auto steps, auto coordinate) {
    std::array<char, 4>  directions =  {'E', 'S', 'W', 'N'};
    std::size_t direction_index = 0;
    for (auto& [action, amount] : steps) {
      if (action == 'F') action = directions[direction_index];
      switch (action) {
      case 'N': { coordinate += {0, +amount}; break; }
      case 'S': { coordinate += {0, -amount}; break; }
      case 'E': { coordinate += {-amount, 0}; break; }
      case 'W': { coordinate += {+amount, 0}; break; }
      case 'L': {
        direction_index =
	  ((direction_index - (amount / 90)) + directions.size()) % directions.size();
	break;
      }
      case 'R': {
        direction_index =
	  ((direction_index + (amount / 90)) + directions.size()) % directions.size();
	break;
      }
      }
    }
    return coordinate;
  }
}

auto manhattan_distance(const auto& from, const auto& to) {
  return std::abs(from.x - to.x)
       + std::abs(from.y - to.y);
}

TEST_CASE("day twelve") {
  const Coordinate origin {0,0};
  
  SECTION("example") {
    auto input = parse(std::stringstream {
R"(F10
N3 
F7
R90
F11)"
    });
    const auto& destination = part_one::navigate(input, origin);
    REQUIRE(manhattan_distance(destination, origin) == 25);
  }
  SECTION("part one") {
    auto input = parse(std::ifstream { "input/day-12.input"});
    const auto& destination = part_one::navigate(input, origin);
    REQUIRE(manhattan_distance(destination, origin) == 439);
  } 
}
