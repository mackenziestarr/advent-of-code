#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <fstream>
#include <ranges>
#include <iostream>
#include <unordered_set>
#include <algorithm>

struct vec3 {
  int x, y, z;
  
  auto adjacents() const {
    std::vector<vec3> adjacents;
    for (int x_offset : {-1, 0, 1}) {
      for (int y_offset : {-1, 0, 1}) {
	for (int z_offset : {-1, 0, 1}) {
	  if (x_offset == 0 && y_offset == 0 && z_offset == 0) {
	    continue;
	  }
	  adjacents.push_back({
	      x + x_offset,
	      y + y_offset,
	      z + z_offset
	  });
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

  bool operator==(const vec3& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }

  struct hash_fn {
    std::size_t operator()(const vec3& v) const {
      return std::hash<int>()(v.x)
           ^ std::hash<int>()(v.y) << 1
      	   ^ std::hash<int>()(v.z) << 2;
    }
  };
  
};

std::ostream& operator<<(std::ostream& os, const vec3& v) {
  os << '{' << v.x << ',' << v.y << ',' << v.z << '}';
  return os;
}

constexpr auto parse = [](auto&& in) {
  std::unordered_set<vec3, vec3::hash_fn> out;
  std::string line;
  int y = 0;
  while (std::getline(in, line)) {
    int x = 0;
    for (auto c : line) {
      if (c == '#') out.insert({x, y, 0});
      x++;
    }
    y++;
  }
  return out;
};

bool is_next_active(const auto& vec3, const auto& set) {
  std::size_t active_neighbors = std::ranges::count_if(
    vec3.adjacents(),
    [&set](const auto& adjacent) {
      return set.contains(adjacent);
    }
  );
  if (set.contains(vec3)) {
    return active_neighbors == 2
        || active_neighbors == 3;
  }
  return active_neighbors == 3;
}

auto simulate(auto input) {
  std::unordered_set<vec3, vec3::hash_fn> next_state;
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
  SECTION("example input") {
    std::stringstream example_input { ".#.\n..#\n###" };
    auto map = parse(example_input);
    for (int cycles = 0; cycles < 6; cycles++) {
      map = simulate(map);
    }
    REQUIRE(map.size() == 112);
  }
  SECTION("part one") {
    auto map = parse(std::ifstream{"input/day-17.input"});
    for (int cycles = 0; cycles < 6; cycles++) {
      map = simulate(map);
    }
    REQUIRE(map.size() == 295);
  }
}
