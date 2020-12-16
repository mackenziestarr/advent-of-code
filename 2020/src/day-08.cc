#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>

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

enum class ProgramStatus {
  Terminated,
  Success
};

OpCode to_opcode(const std::string& op) {
  if (op == "acc") return OpCode::Acc;
  if (op == "nop") return OpCode::Nop;
  if (op == "jmp") return OpCode::Jmp;
  throw std::runtime_error("unsupported opcode: " + op);
}

class VirtualMachine {
  int accumulator = 0;
  std::vector<Instruction>& instructions;
public:
  VirtualMachine(std::vector<Instruction>& instructions)
    : instructions{instructions} {}
  int getAccumulator() const {
    return accumulator;
  }
  ProgramStatus run() {
    std::size_t pc = 0;
    while (pc < instructions.size()) {
      Instruction& i = instructions[pc];

      if (i.executed) {
	return ProgramStatus::Terminated;
      }
      
      switch (to_opcode(i.opcode)) {
        case OpCode::Jmp : {
	  pc += i.argument;
  	  break;
        }
        case OpCode::Acc : {
  	  accumulator += i.argument;
        }
        case OpCode::Nop : {}
        default: pc++;
      }
      
      i.executed = true;
    }
    return ProgramStatus::Success;
  }
};

std::vector<Instruction> parse(const std::string& input) {
  std::ifstream in {input};
  if (!in.good()) throw std::runtime_error("couldn't open file");

  std::vector<Instruction> out;
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream lstream {line};
    Instruction i;
    lstream >> i.opcode >> i.argument;
    out.push_back(i);
  }
  return out;
}

TEST_CASE("part one") {
  auto input = parse("input/day-08.input");
  VirtualMachine vm { input };
  ProgramStatus status = vm.run();
  REQUIRE(status == ProgramStatus::Terminated);
  REQUIRE(vm.getAccumulator() == 1782);
}
