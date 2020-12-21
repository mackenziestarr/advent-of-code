#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <fstream>
#include "lib.h"

enum class Type {
  EmptySeat,
  OccupiedSeat,
  Floor
};

struct Cell {
  Type type;
};

constexpr auto parse = [](auto&& in) {
  return aoc::parse(in, [](const std::string& line) {
    std::vector<Cell> out;
    for (const auto& c : line) {
      if (c == '.') out.push_back({Type::Floor});
      if (c == 'L') out.push_back({Type::EmptySeat});
    }
    return out;
  });
};

int adjacent_occupied_seats(const auto& v, int row_index, int col_index) {
  static const std::pair<int, int> neighbors[] {
    {-1, -1},
    {-1,  0},
    {-1,  1},
    {0,  -1},
    {0,   1},
    {1,  -1},
    {1,   0},
    {1,   1}
  };
  int sum = 0;
  for (const auto& [row_offset, col_offset] : neighbors) {
    auto neighbor_row_index = row_index + row_offset;
    auto neighbor_col_index = col_index + col_offset;
    if (neighbor_row_index < 0 || !(neighbor_row_index < v.size())) continue;
    if (neighbor_col_index < 0 || !(neighbor_col_index < v[0].size())) continue;
    const Cell c = v[neighbor_row_index][neighbor_col_index];
    if (c.type == Type::OccupiedSeat) sum++;
  }
  return sum;
}

auto simulate(const auto& input, bool& was_modified) {
  int modify_count = 0;
  std::vector<std::vector<Cell>> out(input.begin(), input.end());
  for (int i = 0; i < out.size(); i++) {
    for (int j = 0; j < out[0].size(); j++) {
      auto count = adjacent_occupied_seats(input, i, j);
      const auto cell = input[i][j];
      if (cell.type == Type::EmptySeat && count == 0) {
	out[i][j] = {Type::OccupiedSeat};
	modify_count++;
      }
      else if (cell.type == Type::OccupiedSeat && count >= 4) {
	out[i][j] = {Type::EmptySeat};
	modify_count++;	
      }
    }
  }
  was_modified = modify_count > 0;
  return out;
};

auto count_occupied_seats(const auto& input) {
  // TODO replace with flat map
  int sum  = 0;
  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[0].size(); j++) {
      const auto cell = input[i][j];
      if (cell.type == Type::OccupiedSeat) sum++;
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
    bool was_modified = true;
    std::vector<std::vector<Cell>> out = input;
    while (was_modified) {
      out = simulate(out, was_modified);
    }
    REQUIRE(count_occupied_seats(out) == 37);
  }
  SECTION("part one") {
    auto input = parse(std::ifstream { "input/day-11.input" });
    bool was_modified = true;
    std::vector<std::vector<Cell>> out = input;
    while (was_modified) {
      out = simulate(out, was_modified);
    }
    REQUIRE(count_occupied_seats(out) == 2334);
  }
}
