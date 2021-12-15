#ifndef RISC_LAB_INSTRUCTIONS_H
#define RISC_LAB_INSTRUCTIONS_H
#include <cstdint>
#include "string"

const char* reg_ABI(uint8_t reg) {
    switch (reg) {
        case 0: return "zero";
        case 1: return "ra";
        case 2: return "sp";
        case 3: return "gp";
        case 4: return "tp";
        case 5: return "t0";
        case 6: return "t1";
        case 7: return "t2";
        case 8: return "s0";
        case 9: return "s1";
        case 10: return "a0";
        case 11: return "a1";
        case 12: return "a2";
        case 13: return "a3";
        case 14: return "a4";
        case 15: return "a5";
        case 16: return "a6";
        case 17: return "a7";
        case 18: return "s2";
        case 19: return "s3";
        case 20: return "s4";
        case 21: return "s5";
        case 22: return "s6";
        case 23: return "s7";
        case 24: return "s8";
        case 25: return "s9";
        case 26: return "s10";
        case 27: return "s11";
        case 28: return "t3";
        case 29: return "t4";
        case 30: return "t5";
        case 31: return "t6";
        default: return "undefined";
    }
}


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
