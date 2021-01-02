#include <functional>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

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
  template <typename T = int>
  auto split(const std::string& input, const char delimiter) {
    std::vector<T> out;
    std::stringstream ss {input};
    for (T i; ss >> i;) {
      out.push_back(i);
      if (ss.peek() == delimiter) ss.ignore();
    }
    return out;
  }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  out << "[";
  for (std::size_t i = 0; i < v.size(); i++) {
    out << v[i];
    if (i != v.size() - 1) out << ", ";
  }
  out << "]";
  return out;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<K,V>& in) {
  out << "{";
  for (auto it = in.begin(); it != in.end();) {
    const auto& [k, v] = *it;
    out << "{" << k << " => " << v << "}";
    if (++it == in.end()) {}
    else out << ",";
  }
  out << "}";
  return out;
}


