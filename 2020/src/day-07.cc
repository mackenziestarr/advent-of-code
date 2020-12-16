#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using EdgeType  = std::pair<std::size_t, std::string>;
using EdgesType = std::vector<EdgeType>;
using NodeType  = std::pair<std::string, EdgesType>;

NodeType parse_line(const std::string &input) {
  const std::regex re {"(\\w+ \\w+) bags contain (.*)"};
  std::smatch m;
  std::regex_match(input, m, re);
  auto parent = m[1].str();
  auto rest = m[2].str();
  if (rest == "no other bags.")
    return {parent, {}};

  const std::regex re2 {"(\\d) (\\w+ \\w+)"};
  EdgesType children;
  std::transform(
    std::sregex_iterator(rest.cbegin(), rest.cend(), re2),
    std::sregex_iterator(),
    std::back_inserter(children),
    [](const auto& m) -> EdgeType {
      std::size_t num = std::stoi(m[1].str());
      std::string color = m[2].str();
      return {num, color};
    });
  return {parent, children};
}

auto parse(const std::string &input_file) {
  std::ifstream in(input_file);
  if (!in) throw std::runtime_error("couldn't open file");
  std::unordered_map<NodeType::first_type, NodeType::second_type> out;
  std::string line;
  while (std::getline(in, line)) out.insert(parse_line(line));
  return out;
}

auto has_path(const std::unordered_map<NodeType::first_type, NodeType::second_type>& graph,
	      const std::string &start_node,
	      const std::string &end_node)
{
  std::deque<std::string> nodes_to_visit { start_node };
  while (!nodes_to_visit.empty()) {
    std::string current_node = nodes_to_visit.front();
    nodes_to_visit.pop_front();
    if (current_node == end_node) return true;
    for (const auto &[number, edge] : graph.find(current_node)->second) {
      nodes_to_visit.push_back(edge);
    }
  }
  return false;
}

int count_bags(const std::unordered_map<NodeType::first_type, NodeType::second_type>& graph,
	       const std::string &start_node)
{
  int sum = 1;
  for (const auto& [num, edge] : graph.find(start_node)->second) {
    sum += num * count_bags(graph, edge);
  }
  return sum;
}

TEST_CASE("part one") {
  auto input = parse("input/day-07.input");
  std::string target_bag = "shiny gold";
  int count = std::count_if(
    input.cbegin(),
    input.cend(),
    [&input, &target_bag](const auto& pair) {
      const auto& [key, _] = pair;
      if (key == target_bag) return false;
      return has_path(input, key, target_bag);
    }
  );
  REQUIRE(count == 226);
}

TEST_CASE("part two") {
  auto input = parse("input/day-07.input");  
  REQUIRE(count_bags(input, "shiny gold") - 1 == 9569);
}
