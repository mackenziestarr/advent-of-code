#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <iterator>

std::vector<std::string> parse(const std::string &input_file) {
  std::ifstream in(input_file);
  if (!in)
    throw std::runtime_error("couldn't open file");
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

std::map<std::string, std::string> parse_fields(const std::string &input) {
  std::istringstream iss(input);

  std::vector<std::string> fields((std::istream_iterator<std::string>(iss)),
                                  std::istream_iterator<std::string>());

  std::map<std::string, std::string> out;
  std::transform(fields.cbegin(), fields.cend(),
                 std::inserter(out, out.begin()), [](const std::string &s) {
                   return std::make_pair(s.substr(0, s.find_first_of(":")),
                                         s.substr(s.find_first_of(":") + 1));
                 });
  return out;
}

bool includes_required_fields(std::map<std::string, std::string> records) {
  const std::set<std::string> required_fields = {"ecl", "pid", "eyr", "hcl",
                                                 "byr", "iyr", "hgt"};
  std::set<std::string> keys;
  std::transform(records.cbegin(), records.cend(),
                 std::inserter(keys, keys.begin()),
                 [](const auto &field) { return field.first; });
  return std::includes(keys.cbegin(), keys.cend(), required_fields.cbegin(),
                       required_fields.cend());
}

bool is_valid(std::map<std::string, std::string> input) {
  int byr = std::stoi(input["byr"]);
  if (byr < 1920 || byr > 2002)
    return false;
  int iyr = std::stoi(input["iyr"]);
  if (iyr < 2010 || iyr > 2020)
    return false;
  int eyr = std::stoi(input["eyr"]);
  if (eyr < 2020 || eyr > 2030)
    return false;

  const auto &height = input["hgt"];
  const auto &unit = height.substr(height.size() - 2);
  if (unit != "cm" && unit != "in")
    return false;
  const auto value = std::stoi(height.substr(0, height.size() - 2));
  if (unit == "cm" && (value < 150 || value > 193))
    return false;
  if (unit == "in" && (value < 59 || value > 76))
    return false;

  const auto &hair_color = input["hcl"];
  if (hair_color[0] != '#')
    return false;
  for (const auto c : hair_color.substr(1)) {
    if ((c < '0' || c > '9') && (c < 'a' || c > 'f')) {
      return false;
    }
  }

  const std::set<std::string> colors = {"amb", "blu", "brn", "gry",
                                        "grn", "hzl", "oth"};
  if (!colors.contains(input["ecl"]))
    return false;

  const std::string &passport_id = input["pid"];
  if (passport_id.size() != 9)
    return false;
  for (const auto c : passport_id) {
    if (c < '0' && c > '9')
      return false;
  }

  return true;
}

TEST_CASE("day four") {
  const auto batches = parse("input/day-04.input");
  std::vector<std::map<std::string, std::string>> records;
  std::transform(batches.cbegin(), batches.cend(), std::back_inserter(records),
                 parse_fields);
  SECTION("part one") {
    const int answer = std::count_if(records.cbegin(), records.cend(),
                                     includes_required_fields);
    REQUIRE(answer == 210);
  }
  SECTION("part two") {
    std::vector<std::map<std::string, std::string>> valid_records;
    std::copy_if(records.cbegin(), records.cend(),
                 std::back_inserter(valid_records), includes_required_fields);
    const int answer =
        std::count_if(valid_records.cbegin(), valid_records.cend(), is_valid);
    REQUIRE(answer == 131);
  }
}
