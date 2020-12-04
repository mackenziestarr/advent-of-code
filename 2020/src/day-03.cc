#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

enum class Square {
  Empty,
  Tree
};

struct Point {
  int x, y;
};

std::vector<std::vector<Square>> parse(const std::string& input_file) {
  std::ifstream in(input_file);
  if (!in) throw std::runtime_error("couldn't open file");
  std::vector<std::vector<Square>> out;
  std::string line;
  while (std::getline(in, line)) {
    std::vector<Square> row;
    std::transform(
      line.cbegin(),
      line.cend(),
      std::back_inserter(row),
      [](char c) { return c == '.' ? Square::Empty : Square::Tree; }
    );
    out.push_back(row);		     
  }
  return out;
}

std::size_t count_trees_for_slope(
  const std::vector<std::vector<Square>>& input,
  std::size_t right,
  std::size_t down) {
  std::size_t  output = 0;
  Point p = {0,0};
  while (p.y < input.size()) {
    if (input[p.y][p.x] == Square::Tree) {
      output += 1;
    }
    p.y += down;
    p.x = (p.x + right) % input[0].size();
  }
  return output;
}

TEST_CASE("part one") {
  std::vector<std::vector<Square>> input = parse("src/day-03.input");
  std::size_t answer = count_trees_for_slope(input, 3, 1);
  REQUIRE(answer == 286);
}

TEST_CASE("part two") {
  std::vector<std::vector<Square>> input = parse("src/day-03.input");
  std::vector<std::pair<std::size_t, std::size_t>> slopes = {
    {1,1},{3,1},{5,1},{7,1},{1,2}
  };
  auto answer = std::transform_reduce(
	slopes.cbegin(),
	slopes.cend(),
	std::size_t{1},
	std::multiplies<>(),
	[&input](const std::pair<std::size_t, std::size_t>& p) {
	  return count_trees_for_slope(input, p.first, p.second);
	}
  );
  REQUIRE(answer == 3638606400);
}

