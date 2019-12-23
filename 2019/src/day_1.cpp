#include <fstream>
#include <iostream>

namespace part_one {
int fuel_requirement(int mass) { return (mass / 3) - 2; }
} // namespace part_one

namespace part_two {
int fuel_requirement(int mass) {
  int fuel = (mass / 3) - 2;
  if (fuel < 1)
    return 0;
  else
    return fuel + fuel_requirement(fuel);
}
} // namespace part_two

int main() {
  int mass = 0, part_one_total = 0, part_two_total = 0;
  std::ifstream infile("input/day-1.txt");
  while (infile >> mass) {
    part_one_total += part_one::fuel_requirement(mass);
    part_two_total += part_two::fuel_requirement(mass);
  }
  std::cout << "advent of code 2019: day 1" << std::endl;
  std::cout << "part one:\t" << part_one_total << std::endl;
  std::cout << "part two:\t" << part_two_total << std::endl;
  return 0;
}
