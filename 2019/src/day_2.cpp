#define CATCH_CONFIG_RUNNER
#include "../third_party/catch.hpp"

#include <sstream>
#include <string>
#include <vector>

std::vector<int> split(const std::string &input, char delimiter) {
  std::vector<int> v;
  std::stringstream ss(input);
  for (std::string character; std::getline(ss, character, delimiter); ) {
    v.push_back(std::move(std::stoi(character)));
  }
  return v;
}

std::vector<int> process(std::string input) {
  return split(input, ',');
}


int main(int argc, char* argv[]) {
  int result = Catch::Session().run( argc, argv );
  
  return result;
}

TEST_CASE("correctly processes Intcode programs", "[]") {

  std::vector<int> a {2, 0, 0, 0, 99};
  REQUIRE(process("1,0,0,0,99") == a);

  std::vector<int> b {2, 3, 0, 6, 99};
  REQUIRE(process("2,3,0,3,99") == b);

  std::vector<int> c {2, 4, 4, 5, 99, 9801};
  REQUIRE(process("2,4,4,5,99,0") == c);

  std::vector<int> d {30, 1, 1, 4, 2, 5, 6, 0, 99};  
  REQUIRE(process("1,1,1,4,99,5,6,0,99") == d);

}

