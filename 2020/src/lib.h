#include <vector>
#include <istream>
#include <string>
#include <functional>

namespace aoc {
  template <typename Functor>
  auto parse(auto&& istream, Functor fn) {
    if (!istream.good()) {
      throw std::runtime_error("couldn't open input file");
    }
    std::string line;
    std::vector<decltype(fn(std::string {}))> out;
    while(std::getline(istream, line)) out.push_back(fn(line));
    return out;
  }
}

