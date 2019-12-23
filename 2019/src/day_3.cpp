#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct Point {
  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}
  int x, y;
};

struct Segment {
  Point start, end;
  Segment(Point start, Point end) : start(start), end(end) {}
  bool contains(const Point &p) const {
    return std::min(start.x, end.x) <= p.x && std::max(start.x, end.x) >= p.x &&
           std::min(start.y, end.y) <= p.y && std::max(start.y, end.y) >= p.y;
  }
};

std::ostream &operator<<(std::ostream &os, const Point &p) {
  return os << "(" << p.x << "," << p.y << ")";
}
std::ostream &operator<<(std::ostream &os, const Segment &s) {
  return os << "[" << s.start << "," << s.end << "]";
}
bool operator==(const Point &left, const Point &right) {
  return left.x == right.x && left.y == right.y;
}
bool operator!=(const Point &left, const Point &right) {
  return !(left == right);
}
bool operator==(const Segment &left, const Segment &right) {
  return left.start == right.start && left.end == right.end;
}

std::vector<Segment> parse(std::string input) {
  std::vector<Segment> segments;
  std::istringstream iss{input};
  std::string token;
  Point p1, p2;
  while (std::getline(iss, token, ',')) {
    std::istringstream tstream{token};
    char direction;
    int amount;
    tstream >> direction >> amount;
    switch (direction) {
    case 'U':
      p2.y += amount;
      break;
    case 'D':
      p2.y -= amount;
      break;
    case 'R':
      p2.x += amount;
      break;
    case 'L':
      p2.x -= amount;
      break;
    default:
      std::string msg = "unexpected value: ";
      throw std::runtime_error(msg += direction);
    }
    segments.emplace_back(p1, p2);
    p1 = p2;
  }
  return segments;
}

int manhattan_distance(Point p1, Point p2) {
  return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

std::tuple<int, int, int> standard_form_coefficients(const Segment &s) {
  int A = s.end.y - s.start.y;
  int B = s.start.x - s.end.x;
  int C = (A * s.start.x) + (B * s.start.y);
  return std::make_tuple(A, B, C);
}

std::optional<Point> intersection(const Segment &a, const Segment &b) {
  auto [A1, B1, C1] = standard_form_coefficients(a);
  auto [A2, B2, C2] = standard_form_coefficients(b);
  double determinant = (A1 * B2) - (A2 * B1);
  if (determinant == 0) {
    return {};
  }
  int x = ((B2 * C1) - (B1 * C2)) / determinant;
  int y = ((A1 * C2) - (A2 * C1)) / determinant;
  Point p = Point(x, y);
  if (a.contains(p) && b.contains(p)) {
    return p;
  }
  return {};
}

std::vector<Point> intersections(const std::vector<Segment> &xs,
                                 const std::vector<Segment> &ys) {
  std::vector<Point> output;
  for (const auto x : xs) {
    for (const auto y : ys) {
      std::optional<Point> p = intersection(x, y);
      if (p.has_value())
        output.push_back(p.value());
    }
  }
  return output;
}

#ifdef RUN_TESTS
#define CATCH_CONFIG_MAIN
#include "../third_party/catch.hpp"
TEST_CASE("finds intersection of two line segments", "[intersection]") {
  {
    Segment a = Segment(Point(0, 0), Point(0, 5));
    Segment b = Segment(Point(0, 0), Point(5, 0));
    std::optional<Point> p = intersection(a, b);
    REQUIRE(p.value() == Point{0, 0});
  }
  {
    Segment a = Segment(Point(3, -3), Point(3, 3));
    Segment b = Segment(Point(0, 2), Point(10, 2));
    std::optional<Point> p = intersection(a, b);
    REQUIRE(p.value() == Point{3, 2});
  }
  {
    Segment a = Segment(Point(0, 0), Point(0, 5));
    Segment b = Segment(Point(2, 0), Point(2, 5));
    std::optional<Point> p = intersection(a, b);
    REQUIRE(p.has_value() == false);
  }
  {
    Segment a = Segment(Point(0, 0), Point(0, 5));
    Segment b = Segment(Point(-5, -5), Point(-5, 5));
    std::optional<Point> p = intersection(a, b);
    REQUIRE(p.has_value() == false);
  }
}
TEST_CASE("parses vector description notation into a list of line segments",
          "[parse]") {
  std::vector<Segment> s = {
      Segment(Point(0, 0), Point(8, 0)), Segment(Point(8, 0), Point(8, 5)),
      Segment(Point(8, 5), Point(3, 5)), Segment(Point(3, 5), Point(3, 2))};
  REQUIRE(parse("R8,U5,L5,D3") == s);
}
TEST_CASE("calculates manhattan distance between two points",
          "[manhattan_distance]") {
  REQUIRE(manhattan_distance(Point(0, 0), Point(3, 3)) == 6);
}
TEST_CASE(
    "returns a list of intersecting points for two lists of line segments",
    "[intersections]") {
  std::vector<Segment> a = {
      Segment(Point(0, 0), Point(8, 0)), Segment(Point(8, 0), Point(8, 5)),
      Segment(Point(8, 5), Point(3, 5)), Segment(Point(3, 5), Point(3, 2))};
  std::vector<Segment> b = {
      Segment(Point(0, 0), Point(0, 7)), Segment(Point(0, 7), Point(6, 7)),
      Segment(Point(6, 7), Point(6, 3)), Segment(Point(6, 3), Point(2, 3))};
  REQUIRE(intersections(a, b) ==
          std::vector<Point>{Point(0, 0), Point(6, 5), Point(3, 3)});
}
#else
int main() {
  std::ifstream ifs("./input/day-3.txt");
  if (!ifs) {
    std::cerr << "couldn't open file for reading" << '\n';
    return 1;
  }

  std::string line;
  std::getline(ifs, line);
  auto segment_a = parse(line);
  std::getline(ifs, line);
  auto segment_b = parse(line);

  std::cout << "advent of code 2019: day 3" << '\n';

  int minimum_distance = std::numeric_limits<int>::max();
  std::vector<int> distances;
  const Point ORIGIN;
  for (auto intersection : intersections(segment_a, segment_b)) {
    if (intersection != ORIGIN) {
      int distance = manhattan_distance(intersection, ORIGIN);
      minimum_distance = std::min(minimum_distance, distance);
    }
  }
  std::cout << "part one:\t" << minimum_distance << '\n';

  int minimum_delay = std::numeric_limits<int>::max();
  int a_dist = 0;
  for (const auto a : segment_a) {
    int b_dist = 0;
    for (const auto b : segment_b) {
      std::optional<Point> p = intersection(a, b);
      if (p.has_value()) {
        if (p != ORIGIN) {
          int intersection_distance = a_dist + b_dist +
                                      manhattan_distance(p.value(), a.start) +
                                      manhattan_distance(p.value(), b.start);
          minimum_delay = std::min(minimum_delay, intersection_distance);
        }
      }
      b_dist += manhattan_distance(b.start, b.end);
    }
    a_dist += manhattan_distance(a.start, a.end);
  }

  std::cout << "part two:\t" << minimum_delay << '\n';

  return 0;
}
#endif
