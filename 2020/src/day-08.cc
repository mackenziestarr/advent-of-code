#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include "lib.h"

struct Instruction {
  std::string opcode;
  int argument;
  bool executed = false;
};

enum class OpCode {
  Acc,
  Nop,
  Jmp
};

enum class Status {
  Error,
  Success
};

struct Result {
  Status status;
  int accumulator = 0;
};

OpCode to_opcode(const std::string& op) {
  if (op == "acc") return OpCode::Acc;
  if (op == "nop") return OpCode::Nop;
  if (op == "jmp") return OpCode::Jmp;
  throw std::runtime_error("unsupported opcode: " + op);
}

Result run(std::vector<Instruction> instructions) {
  std::size_t pc = 0;
  int accumulator = 0;
  while (pc < instructions.size()) {
    Instruction& i = instructions[pc];

    if (i.executed) {
      return {Status::Error, accumulator};
    }
      
    switch (to_opcode(i.opcode)) {
      case OpCode::Jmp : {
        pc += i.argument;
        break;
      }
      case OpCode::Acc : {
        accumulator += i.argument;
        pc++;
        break;
      }
      case OpCode::Nop : {
        pc++;
        break;
      }
    }
      
    i.executed = true;
  }

  return {Status::Success, accumulator};
}

Result repair(std::vector<Instruction>& instructions) {
  for (auto& [opcode, _arg, _ex] : instructions) {
    if (opcode == "jmp") opcode = "nop";
    else if (opcode == "nop") opcode = "jmp";
    else continue;

    const auto& res = run(instructions);
    if (res.status == Status::Success) return res;
    else opcode = opcode == "jmp" ? "nop" : "jmp";
  }
  return {Status::Error, 0};
}

TEST_CASE("day 8") {
  auto instructions = parse(
    std::ifstream{"input/day-08.input"},
    [](const std::string& line) -> Instruction {
      return {line.substr(0, 3), std::atoi(line.c_str() + 3)};
    }
  );
  SECTION("part one") {
    const auto& [status, acc] = run(instructions);
    REQUIRE(status == Status::Error);
    REQUIRE(acc == 1782);
  }
  SECTION("part two") {
    const auto& [status, acc] = repair(instructions);
    REQUIRE(status == Status::Success);
    REQUIRE(acc == 797);
  }
}
