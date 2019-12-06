#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

constexpr size_t OpCodeAdd = 1;
constexpr size_t OpCodeMultiply = 2;
constexpr size_t OpCodeHalt = 99;

std::vector<int> split(const std::string& input, const char delimiter) {
  std::vector<int> v;
  std::stringstream ss(input);
  for (std::string character; std::getline(ss, character, delimiter); ) {
    v.push_back(std::move(std::stoi(character)));
  }
  return v;
}

std::vector<int> execute(std::vector<int>& tokens) {
  for (auto i = 0; i < tokens.size() && tokens[i] != OpCodeHalt; i+=4) {
    int opcode	= tokens[i];
    int left	= tokens[i+1];
    int right	= tokens[i+2];
    int result	= tokens[i+3];
    if (opcode == OpCodeAdd) {
      tokens[result] = tokens[left] + tokens[right];      
    }
    else if (opcode == OpCodeMultiply) {
      tokens[result] = tokens[left] * tokens[right];
    }
    else {
      throw std::runtime_error("unexpected opcode: " + std::to_string(opcode));
    }
  }
  return tokens;
}

#ifdef RUN_TESTS
#define CATCH_CONFIG_MAIN
#include "../third_party/catch.hpp"
TEST_CASE("correctly executees Intcode programs", "[execute]") {
  {
    std::vector<int> input {1, 0, 0, 0, 99};
    std::vector<int> expected {2, 0, 0, 0, 99};
    REQUIRE(execute(input) == expected);
  }
  {
    std::vector<int> input {2, 3, 0, 3, 99};
    std::vector<int> expected {2, 3, 0, 6, 99};
    REQUIRE(execute(input) == expected);
  }
  {
    std::vector<int> input {2, 4, 4, 5, 99, 0};
    std::vector<int> expected {2, 4, 4, 5, 99, 9801};
    REQUIRE(execute(input) == expected);
  }
  {
    std::vector<int> input {1, 1, 1, 4, 99, 5, 6, 0, 99};
    std::vector<int> expected {30, 1, 1, 4, 2, 5, 6, 0, 99};  
    REQUIRE(execute(input) == expected);
  }
}
#else
int part_one(std::vector<int> tokens) {
  // restore the gravity assist program (your puzzle input) to the
  // "1202 program alarm" state it had just before the last computer
  // caught fire.
  tokens[1] = 12;
  tokens[2] = 2;
  std::vector<int> output = execute(tokens);
  return output[0];
}
int part_two(std::vector<int> tokens) {
  std::vector<int> memory;
  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      memory = tokens;
      memory[1] = noun;
      memory[2] = verb;
      std::vector<int> output = execute(memory);
      if (output[0] == 19690720) {
	return 100 * noun + verb;
      }
    }
  }
  throw std::runtime_error("couldn't find solution");
}

int main() {
  std::ifstream infile("./input/day-2.txt");
  std::string input;
  infile >> input;
  std::vector<int> tokens = split(input, ',');
  std::cout << "advent of code 2019: day 2" << std::endl;
  std::cout << "part one:\t" << part_one(tokens) << std::endl;
  std::cout << "part two:\t" << part_two(tokens) << std::endl;    
  return 0;
}
#endif
