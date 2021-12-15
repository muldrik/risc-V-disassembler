#ifndef RISC_LAB_INSTRUCTIONS_H
#define RISC_LAB_INSTRUCTIONS_H
#include <cstdint>
#include "string"

const char* reg_ABI(uint8_t reg);

enum I_type {
    R, I, S, B, U, J, Il, Undef
};

struct Instruction {
    const char* name = nullptr;
    std::string label;
    I_type type = Undef;
    uint8_t opcode = 0;
    uint8_t rd = 0;
    uint8_t funct3 = 0;
    uint8_t rs1 = 0;
    uint8_t rs2 = 0;
    uint8_t funct2 = 0;
    uint8_t funct5 = 0;
};

Instruction parse_instruction(uint16_t b1, uint16_t b2);
#endif //RISC_LAB_INSTRUCTIONS_H
