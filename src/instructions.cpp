#include "instructions.h"


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

Instruction parse_instruction(uint16_t b1, uint16_t b2) {
    Instruction i;
    i.opcode = (b1 & 0b1111111);
    i.rd = ((b1 >> 7) & (0b11111));
    i.funct3 = (b1 >> 12) & (0b111);
    i.rs1 = ((b1 >> 15)&1) + ((b2 & 0b1111) << 1);
    i.rs2 = (b2 >> 4) & (0b11111);
    i.funct2 = (b2 >> 9) & (0b11);
    i.funct5 = (b2 >> 11) & (0b11111);
    switch (i.opcode) {
        case 0b110111: {i.type = U; i.name = "lui"; break;}
        case 0b10111: {i.type = U; i.name = "auipc"; break;}
        case 0b1101111: {i.type = J; i.name = "jal"; break;}
        case 0b1100111: {i.type = I, i.name = "jalr"; break;}
        case 0b1100011: {
            i.type = B;
            switch (i.funct3) {
                case 0: {i.name = "beq"; break;}
                case 1: {i.name = "bne"; break;}
                case 4: {i.name = "blt"; break;}
                case 5: {i.name = "bge"; break;}
                case 6: {i.name = "bltu"; break;}
                case 7: {i.name = "bgeu"; break;}
            }
            break;
        }
        case 0b11: {
            i.type = Il;
            switch (i.funct3) {
                case 0: {i.name = "lb"; break;}
                case 1: {i.name = "lh"; break;}
                case 2: {i.name = "lw"; break;}
                case 4: {i.name = "lbu"; break;}
                case 5: {i.name = "lhu"; break;}
            }
            break;
        }
        case 0b100011: {
            i.type = S;
            switch (i.funct3) {
                case 0: {i.name = "sb"; break;}
                case 1: {i.name = "sh"; break;}
                case 2: {i.name = "sw"; break;}
            }
            break;
        }
        case 0b10011: {
            i.type = I;
            switch (i.funct3) {
                case 0: {i.name = "addi"; break;}
                case 2: {i.name = "slti"; break;}
                case 3: {i.name = "sltiu"; break;}
                case 4: {i.name = "xori"; break;}
                case 6: {i.name = "ori"; break;}
                case 7: {i.name = "andi"; break;}
                case 1: {i.name = "slli"; break;}
                case 5: {
                    if (i.funct2 == 0 && i.funct5 == 0) i.name = "SRLI";
                    else if (i.funct2 == 0 && i.funct5 == 0b1000) i.name = "SRAI";
                    break;
                }
            }
            break;
        }
        case 0b110011: {
            i.type = R;
            if (i.funct2 == 0) switch (i.funct3) {
                    case 0: {
                        if (i.funct2 == 0 && i.funct5 == 0) i.name = "ADD";
                        else if (i.funct2 == 0 && i.funct5 == 0b1000) i.name = "SUB";
                        break;
                    }
                    case 1: {if (i.funct5 == 0) i.name = "sll"; break;}
                    case 2: {if (i.funct5 == 0) i.name = "slt"; break;}
                    case 3: {if (i.funct5 == 0) i.name = "sltu"; break;}
                    case 4: {if (i.funct5 == 0) i.name = "xor"; break;}
                    case 5: {
                        if (i.funct5 == 0) i.name = "srl";
                        else if (i.funct5 ==  0b1000) i.name = "sra";
                        break;
                    }
                    case 6: {if (i.funct5 == 0) i.name = "or"; break;}
                    case 7: {if (i.funct5 == 0) i.name = "and"; break;}
                }
            else if (i.funct2 == 0)
                switch(i.funct3) {
                    case 0: {i.name = "mul"; break;}
                    case 1: {i.name = "mulh"; break;}
                    case 2: {i.name = "mulhsu"; break;}
                    case 3: {i.name = "mulhu"; break;}
                    case 4: {i.name = "div"; break;}
                    case 5: {i.name = "divu"; break;}
                    case 6: {i.name = "rem"; break;}
                    case 7: {i.name = "remu"; break;}
                }
            break;
        }
    }
    return i;

}
