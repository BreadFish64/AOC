#include <fstream>
#include <vector>

#include <fmt/ostream.h>

#include "types.hpp"

enum class Opcode {
    acc,
    jmp,
    nop,
};

std::string_view to_string(Opcode opcode) {
    switch (opcode) {
    case Opcode::acc: return "acc"sv;
    case Opcode::jmp: return "jmp"sv;
    case Opcode::nop: return "nop"sv;
    default: std::abort();
    }
}

struct Instruction {
    Opcode opcode;
    s32 operand;

    friend std::istream& operator>>(std::istream& stream, Instruction& instruction) {
        char op;
        stream >> op;
        switch (op) {
        case 'a': instruction.opcode = Opcode::acc; break;
        case 'j': instruction.opcode = Opcode::jmp; break;
        case 'n': instruction.opcode = Opcode::nop; break;
        default: std::abort();
        }
        stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
        stream >> instruction.operand;
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return stream;
    }
};

class Interpreter {
    std::vector<Instruction> binary;
    s64 accumulator{0};
    u64 pc{0};

public:
    Interpreter(const Interpreter&) = default;
    Interpreter(std::istream& assembly)
        : binary{std::istream_iterator<Instruction>{assembly}, {}} {}
    void Reset() {
        accumulator = 0;
        pc = 0;
    }
    void Patch(u64 pc, Opcode op) { binary[pc].opcode = op; }
    s64 GetAccumulator() { return accumulator; }
    auto RunTillRepeat() {
        std::vector<bool> execution_tracker(binary.size());
        std::vector<std::pair<u64, Opcode>> patches;
        while (!execution_tracker[pc]) {
            execution_tracker[pc] = true;
            auto [opcode, operand] = binary[pc];
            switch (opcode) {
            case Opcode::acc: {
                accumulator += operand;
                ++pc;
            } break;
            case Opcode::jmp: {
                patches.emplace_back(pc, Opcode::nop);
                pc += operand;
            } break;
            case Opcode::nop: {
                patches.emplace_back(pc, Opcode::jmp);
                ++pc;
            } break;
            default: {
                std::abort();
            }
            }
        }
        return patches;
    }
    bool Halts() {
        std::vector<bool> execution_tracker(binary.size());
        while (pc < binary.size()) {
            if (execution_tracker[pc]) return true;
            execution_tracker[pc] = true;
            auto [opcode, operand] = binary[pc];
            switch (opcode) {
            case Opcode::acc: {
                accumulator += operand;
                ++pc;
            } break;
            case Opcode::jmp: {
                pc += operand;
            } break;
            case Opcode::nop: {
                ++pc;
            } break;
            default: {
                std::abort();
            }
            }
        }
        return false;
    }
    void Translate(std::ostream& stream) {
        for (usize pc = 0; pc < binary.size(); ++pc) {
            auto [opcode, operand] = binary[pc];
            switch (opcode) {
            case Opcode::acc: {
                stream << 'L' << pc << ": accumulator += " << operand << ";\n";
            } break;
            case Opcode::jmp: {
                stream << 'L' << pc << ": goto L" << pc + operand << ";\n";
            } break;
            case Opcode::nop: {
                stream << 'L' << pc << ":\n";
            } break;
            default: {
                std::abort();
            }
            }
        }
    }
};

int main() {
    std::ifstream input_file{"input.txt"};
    Interpreter interpreter(input_file);
    auto patches = interpreter.RunTillRepeat();
    fmt::print("Accumulator value at halt: {}\n", interpreter.GetAccumulator());
    interpreter.Reset();
    for (auto [patch_pc, patch_op] : patches) {
        Interpreter interpreter2{interpreter};
        interpreter2.Patch(patch_pc, patch_op);
        if (!interpreter2.Halts()) {
            fmt::print("Patch {:03} {} acc = {}\n", patch_pc, to_string(patch_op),
                       interpreter2.GetAccumulator());
            std::ofstream cpp{"cpp.cpp"};
            interpreter2.Translate(cpp);
        }
    }
}