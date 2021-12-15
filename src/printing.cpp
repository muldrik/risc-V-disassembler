#include "printing.h"

void fprint_symtab_entry(Symtab_entry& e, FILE* out) {
    fprintf(out, "[%4i] 0x%-15X %5i %-8s %-8s %-8s %6s %s\n",
            e.index, e.st_value, e.st_size,
            symtype_to_string(e.st_info).c_str(),
            symbind_to_string(e.st_info).c_str(),
            symvis_to_string(e.st_other).c_str(),
            symindex_to_string(e.st_shndx).c_str(),
            e.name.c_str());
}


void fprint_symtab(std::vector<Symtab_entry>& entries, FILE* out) {
    fprintf(out, ".symtab\n");
    fprintf(out, "%s %-15s %7s %-8s %-8s %-8s %6s %s\n", "Symbol", "Value", "Size", "Type", "Bind", "Vis", "Index", "Name");
    for (auto& entry: entries) {
        fprint_symtab_entry(entry, out);
    }
}



void fprint_R(const Instruction& i, uint32_t addr, FILE* out) {
    fprintf(out, "%08x %10s: %s %s, %s, %s\n",
            addr,
            i.label.c_str(),
            i.name,
            reg_ABI(i.rd), reg_ABI(i.rs1), reg_ABI(i.rs2));
}

void fprint_B(const Instruction& i, uint32_t addr, FILE* out) {
    const uint16_t sign = ((i.funct5 >> 4) ? 1 : 0) << (12);
    int16_t offset = ((((uint16_t) (((i.funct5 & 0b1111) << 2) + i.funct2)) << 5)
                      + (((uint16_t) i.rd & 1) << 11) + (i.rd & 0b11110)) - sign;
    fprintf(out, "%08x %10s: %s %s, %s, %d\n",
            addr,
            i.label.c_str(),
            i.name,
            reg_ABI(i.rs1),
            reg_ABI(i.rs2),
            offset);
}

void fprint_S(const Instruction& i, uint32_t addr, FILE* out) {
    const uint16_t sign = ((i.funct5 >> 4) ? 1 : 0) << 11;
    int16_t offset = ((((uint16_t) (((i.funct5 & 0b1111) << 2) + i.funct2)) << 5)
                      + i.rd) - sign;
    fprintf(out, "%08x %10s: %s %s, %d(%s)\n",
            addr,
            i.label.c_str(),
            i.name,
            reg_ABI(i.rs2),
            offset,
            reg_ABI(i.rs1));
}

void fprint_J(const Instruction& i, uint32_t addr, FILE* out) {
    const uint32_t sign = ((i.funct5 >> 4) ? 1 : 0) << 20;
    int32_t offset = ((((uint32_t) (i.funct5 & 0b1111)) << 7) +
                      (((uint32_t) (i.funct2)) << 5)  +
                      (((uint32_t) (i.rs2)) << 1) +
                      (((uint32_t) (i.rs1 & 1)) << 11) +
                      (((uint32_t) (i.funct3)) << 12)) - sign;
    fprintf(out, "%08x %10s: %s %s, %d\n",
            addr,
            i.label.c_str(),
            i.name,
            reg_ABI(i.rd),
            offset);
}

void fprint_U(const Instruction& i, uint32_t addr, FILE* out) {
    const uint32_t sign = ((i.funct5 >> 4) ? 1 : 0) << 31;
    int32_t offset = ((((uint32_t) (i.funct5 & 0b1111)) << 27) +
                      (((uint32_t) (i.funct2)) << 25)  +
                      (((uint32_t) (i.rs2)) << 20) +
                      (((uint32_t) (i.rs1)) << 15) +
                      (((uint32_t) (i.funct3)) << 12)) - sign;
    fprintf(out, "%08x %10s: %s %s, %s, %d\n",
            addr,
            i.label.c_str(),
            i.name,
            reg_ABI(i.rd),
            reg_ABI(i.rs1),
            offset);
}

void fprint_I(const Instruction& i, uint32_t addr, FILE* out) {
    const uint32_t sign = ((i.funct5 >> 4) ? 1 : 0) << 11;
    int32_t pos = (((((uint16_t) i.funct5 & 0b1111) << 2) + i.funct2) << 5) + i.rs2;
    int32_t imm = pos - sign;
    fprintf(out, "%08x %10s: %s %s, %s, %d\n",
            addr,
            i.label.c_str(),
            i.name,
            reg_ABI(i.rd),
            reg_ABI(i.rs1),
            imm);
}

void fprint_Il(const Instruction& i, uint32_t addr, FILE* out) {
    const uint16_t sign = ((i.funct5 >> 4) ? 1 : 0) << 11;
    int16_t offset = (((((uint16_t) i.funct5 & 0b1111) << 2) + i.funct2) << 5) + i.rs2 - sign;
    fprintf(out, "%08x %10s: %s %s, %d(%s)\n",
            addr,
            i.label.c_str(),
            i.name,
            reg_ABI(i.rd),
            offset,
            reg_ABI(i.rs1));
}

void fprint_unknown(const Instruction& i, uint32_t addr, FILE* out) {
    fprintf(out, "%08x %10s: UNKNOWN\n", addr, "");
}

void fprint_command(const Instruction& i, uint32_t addr, FILE* out) {
    switch (i.type) {
        case U: {fprint_U(i, addr, out); break;}
        case J: {fprint_J(i, addr, out); break;}
        case I: {fprint_I(i, addr, out); break;}
        case B: {fprint_B(i, addr, out); break;}
        case S: {fprint_S(i, addr, out); break;}
        case R: {fprint_R(i, addr, out); break;}
        case Il: {fprint_Il(i, addr, out); break;}
        default:
            fprint_unknown(i, addr, out);
            break;
    }
}
