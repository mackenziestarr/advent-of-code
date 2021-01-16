#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <fstream>
#include <ranges>
#include <iostream>
#include <unordered_set>
#include <algorithm>

struct vec4 {
  int x, y, z, w;
  bool use_4th_dimension = true;
  
  auto adjacents() const {
    std::vector<vec4> adjacents;
    for (int x_offset : {-1, 0, 1}) {
      for (int y_offset : {-1, 0, 1}) {
	for (int z_offset : {-1, 0, 1}) {
	  for (int w_offset : {-1, 0, 1}) {
	    if (x_offset == 0 && y_offset == 0 && z_offset == 0 && w_offset == 0) {
	      continue;
	    }
	    if (!use_4th_dimension && w_offset != 0) continue;
	    vec4 adjacent { x + x_offset, y + y_offset, z + z_offset, w + w_offset };
	    adjacent.use_4th_dimension = use_4th_dimension;
	    adjacents.push_back(adjacent);
	  }
	}
      }
    }
    return adjacents;
  }

  auto adjacents_with_self() const {
    auto result = adjacents();
    result.push_back(*this);
    return result;
  }

  bool operator==(const vec4& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
  }

  struct hash_fn {
    std::size_t operator()(const vec4& v) const {
      return std::hash<int>()(v.x)
           ^ std::hash<int>()(v.y) << 1
      	   ^ std::hash<int>()(v.z) << 2
      	   ^ std::hash<int>()(v.w) << 3;
    }
  };
  
};

constexpr auto parse = [](auto&& in, bool use_4th_dimension = true) {
  std::unordered_set<vec4, vec4::hash_fn> out;
  std::string line;
  int y = 0;
  while (std::getline(in, line)) {
    int x = 0;
    for (auto c : line) {
      if (c == '#') {
	vec4 v = {x, y, 0, 0};
	v.use_4th_dimension = use_4th_dimension;
	out.insert(v);
      }
      x++;
    }
    y++;
  }
  return out;
};

bool is_next_active(const auto& vec4, const auto& set) {
  std::size_t active_neighbors = std::ranges::count_if(
    vec4.adjacents(),
    [&set](const auto& adjacent) {
      return set.contains(adjacent);
    }
  );
  if (set.contains(vec4)) {
    return active_neighbors == 2
        || active_neighbors == 3;
  }
  return active_neighbors == 3;
}

auto simulate(auto input) {
  std::unordered_set<vec4, vec4::hash_fn> next_state;
  for (const auto& active : input) {
    for (const auto& coord : active.adjacents_with_self()) {
      if (is_next_active(coord, input)) {
	next_state.insert(coord);
      }
    }
  }
  return next_state;
}

TEST_CASE("day seventeen") {
  SECTION("example input, part one") {
    auto map = parse(std::stringstream { ".#.\n..#\n###" }, false /* use_4th_dimension */);
    for (int cycles = 0; cycles < 6; cycles++) {
      map = simulate(map);
    }
    REQUIRE(map.size() == 112);
  }
  SECTION("part one") {
    auto map = parse(std::ifstream{"input/day-17.input"}, false /* use_4th_dimension */);
    for (int cycles = 0; cycles < 6; cycles++) {
      map = simulate(map);
    }
    REQUIRE(map.size() == 295);
  }
  SECTION("example input, part two") {
    std::stringstream example_input { ".#.\n..#\n###" };
    auto map = parse(example_input, true /* use_4th_dimension */);
    for (int cycles = 0; cycles < 6; cycles++) {
      map = simulate(map);
    }
    REQUIRE(map.size() == 848);
  }
  SECTION("part two") {
    std::stringstream example_input { ".#.\n..#\n###" };
    auto map = parse(std::ifstream{"input/day-17.input"}, true /* use_4th_dimension */);    
    for (int cycles = 0; cycles < 6; cycles++) {
      map = simulate(map);
    }
    REQUIRE(map.size() == 848);
  }
}
