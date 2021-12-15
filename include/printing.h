
#ifndef RISC_V_DISASSEMBLER_PRINTING_H
#define RISC_V_DISASSEMBLER_PRINTING_H


void fprint_symtab_entry(Symtab_entry& e, FILE* out);


void fprint_symtab(std::vector<Symtab_entry>& entries, FILE* out);



void fprint_R(const Instruction& i, uint32_t addr, FILE* out);

void fprint_B(const Instruction& i, uint32_t addr, FILE* out);

void fprint_S(const Instruction& i, uint32_t addr, FILE* out);

void fprint_J(const Instruction& i, uint32_t addr, FILE* out);

void fprint_U(const Instruction& i, uint32_t addr, FILE* out);

void fprint_I(const Instruction& i, uint32_t addr, FILE* out);

void fprint_Il(const Instruction& i, uint32_t addr, FILE* out);

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

#endif //RISC_V_DISASSEMBLER_PRINTING_H
