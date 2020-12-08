#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <fstream>
#include <functional>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::vector<std::set<char>>> parse(const std::string &input_file) {
  std::ifstream in(input_file);
  if (!in) throw std::runtime_error("couldn't open file");

  std::vector<std::vector<std::set<char>>> out;
  std::vector<std::set<char>> group;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      out.push_back(group);
      group.clear();
    } else {
      group.emplace_back(line.begin(), line.end());
    }
  }
  out.push_back(group);
  return out;
}

TEST_CASE("part one") {
  const auto groups = parse("input/day-06.input");
  std::vector<std::set<char>> union_sets;
  std::transform(
    groups.begin(),
    groups.end(),
    std::back_inserter(union_sets),
    [](const auto& v) {
      return std::accumulate(v.begin(), v.end(), std::set<char>{},
        [](const auto& lhs, const auto& rhs) {
	  std::set<char> unioned_set;
	  std::set_union(
            lhs.begin(), lhs.end(),
	    rhs.begin(), rhs.end(),
	    std::inserter(unioned_set, unioned_set.begin())
	  );
	  return unioned_set;
	});
    }
  );
  int answer = std::transform_reduce(
    union_sets.begin(),
    union_sets.end(),
    int{0},
    std::plus<>(),
    [](const auto& set) { return set.size(); });
  REQUIRE(answer == 6170);
}

TEST_CASE("part two") {
  const auto groups = parse("input/day-06.input");
  std::vector<std::set<char>> intersected_sets;
  std::transform(
    groups.begin(),
    groups.end(),
    std::back_inserter(intersected_sets),
    [](const auto& v) {
      return std::accumulate(v.begin() + 1, v.end(), *v.begin(),
        [](const auto& lhs, const auto& rhs) {
	  std::set<char> intersected_set;
	  std::set_intersection(
            lhs.begin(), lhs.end(),
	    rhs.begin(), rhs.end(),
	    std::inserter(intersected_set, intersected_set.begin())
	  );
	  return intersected_set;
	});
    }
  );
  int answer = std::transform_reduce(
    intersected_sets.begin(),
    intersected_sets.end(),    
    int{0},
    std::plus<>(),
    [](const auto& set) { return set.size(); });
  REQUIRE(answer == 2947);
}

