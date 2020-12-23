#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <set>
#include "lib.h"

enum class Position {
  EmptySeat,
  OccupiedSeat,
  Floor
};

constexpr auto parse = [](auto&& in) {
  return aoc::parse(in, [](const std::string& line) {
    std::vector<Position> out;
    for (const auto& c : line) {
      if (c == '.') out.push_back({Position::Floor});
      if (c == 'L') out.push_back({Position::EmptySeat});
      if (c == '#') out.push_back({Position::OccupiedSeat});
    }
    return out;
  });
};

constexpr auto in_bounds = [](const auto& v, int row_index, int col_index) {
  int row_size = v.size();
  int col_size = v[0].size();
  return row_index >= 0 && row_index < row_size &&
         col_index >= 0 && col_index < col_size;
};


auto adjacent_occupied_seats = [](const auto& v, int row_index, int col_index) {
  static const std::pair<int, int> neighbors[] {
    {-1, -1}, {0, -1}, {+1,  -1},
    {-1,  0},          {+1,   0},
    {-1, +1}, {0, +1}, {+1,  +1}
  };
  int sum = 0;
  for (const auto& [row_offset, col_offset] : neighbors) {
    auto neighbor_row_index = row_index + row_offset;
    auto neighbor_col_index = col_index + col_offset;
    if (!in_bounds(v, neighbor_row_index, neighbor_col_index)) continue;
    const auto p = v[neighbor_row_index][neighbor_col_index];
    if (p == Position::OccupiedSeat) sum++;
  }
  return sum;
};

auto visible_occupied_seats = [](const auto& v, int row_index, int col_index) {
  static const std::pair<int, int> neighbors[] {
    {-1, -1}, {0, -1}, {+1,  -1},
    {-1,  0},          {+1,   0},
    {-1, +1}, {0, +1}, {+1,  +1}
  };
  int sum = 0;
  for (const auto& [row_offset, col_offset] : neighbors) {
    auto neighbor_row_index = row_index + row_offset;
    auto neighbor_col_index = col_index + col_offset;
    while (in_bounds(v, neighbor_row_index, neighbor_col_index)) {
      const Position p = v[neighbor_row_index][neighbor_col_index];

      if (p == Position::OccupiedSeat) {
	sum++;
	break;
      } else if (p == Position::EmptySeat) {
	break;
      }

      neighbor_row_index += row_offset;
      neighbor_col_index += col_offset;
    }
  }
  
  return sum;
};


template <typename CountFn>
auto simulate(auto input, int empty_seat_threshold, CountFn fn) {
  while (true) {
    int was_modified = false;
    std::vector<std::vector<Position>> output = input;
    for (int i = 0; i < output.size(); i++) {
	for (int j = 0; j < output[0].size(); j++) {
	  auto count = fn(input, i, j);
	  const auto p = input[i][j];
	  if (p == Position::EmptySeat && count == 0) {
	    output[i][j] = Position::OccupiedSeat;
	    was_modified = true;
	  }
	  else if (p == Position::OccupiedSeat && count >= empty_seat_threshold) {
	    output[i][j] = Position::EmptySeat;
	    was_modified = true;	    
	  }
	}
    }
    if (!was_modified) return output;
    input = output;
  }
  return input;
}

auto count_occupied_seats(const auto& input) {
  int sum  = 0;
  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[0].size(); j++) {
      const auto p = input[i][j];
      if (p == Position::OccupiedSeat) sum++;
    }
  }
  return sum;
}

TEST_CASE("day ten") {
  SECTION("example") {
    auto input = parse(std::stringstream {
      R"(L.LL.LL.LL
         LLLLLLL.LL
         L.L.L..L..
         LLLL.LL.LL
         L.LL.LL.LL
         L.LLLLL.LL
         ..L.L.....
         LLLLLLLLLL
         L.LLLLLL.L
         L.LLLLL.LL)"
    });

    auto part_one_output = simulate(input, 4, adjacent_occupied_seats);
    REQUIRE(count_occupied_seats(part_one_output) == 37);
    
    auto part_two_output = simulate(input, 5, visible_occupied_seats);
    REQUIRE(count_occupied_seats(part_two_output) == 26);
  }
  SECTION("part one") {
    auto input = parse(std::ifstream { "input/day-11.input" });
    auto output = simulate(input, 4, adjacent_occupied_seats);
    REQUIRE(count_occupied_seats(output) == 2334);
  }
  SECTION("part two") {
    auto input = parse(std::ifstream { "input/day-11.input" });
    auto output = simulate(input, 5, visible_occupied_seats);
    REQUIRE(count_occupied_seats(output) == 2100);
  }
}
