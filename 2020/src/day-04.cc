#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>

std::vector<std::string> parse(const std::string& input_file) {
  std::ifstream in(input_file);
  if (!in) throw std::runtime_error("couldn't open file");
  std::vector<std::string> out;
  std::string line, batch;
  while (std::getline(in, line)) {
    if (line.empty()) {
      out.push_back(batch);
      batch = "";
    } else {
      batch += (line + " ");
    }
  }
  out.push_back(batch);
  return out;
}

std::set<std::string> field_names(const std::string& input) {
  std::istringstream iss(input);

  std::vector<std::string> fields(
    (std::istream_iterator<std::string>(iss)),
    std::istream_iterator<std::string>()
  );

  std::set<std::string> out;  
  std::transform(
    fields.cbegin(),
    fields.cend(),
    std::inserter(out, out.begin()),
    [](const std::string& s) {
      return s.substr(0, s.find_first_of(":"));
    }
  );
  return out;
}

TEST_CASE("part one") {
  const auto batches = parse("src/day-04.input");
  const auto std::set<std::string> required_fields = {
    "ecl", "pid", "eyr", "hcl", "byr", "iyr", "hgt"
  };
  const int answer = std::count_if(
    batches.cbegin(),
    batches.cend(),
    [&required_fields](const auto& batch) {
      const auto fields = field_names(batch);
      return std::includes(
        fields.cbegin(), fields.cend(),
	required_fields.cbegin(), required_fields.cend()
      );
    }
  );
  REQUIRE(answer == 210);
}
