
#ifndef RISC_V_DISASSEMBLER_PRINTING_H
#define RISC_V_DISASSEMBLER_PRINTING_H
#include "fstream"
#include "instructions.h"
#include "elf_parsing.h"
#include "cstdint"
#include "iostream"
#include "vector"

void fprint_symtab_entry(Symtab_entry& e, FILE* out);


void fprint_symtab(std::vector<Symtab_entry>& entries, FILE* out);



void fprint_R(const Instruction& i, uint32_t addr, FILE* out);

void fprint_B(const Instruction& i, uint32_t addr, FILE* out);

void fprint_S(const Instruction& i, uint32_t addr, FILE* out);

void fprint_J(const Instruction& i, uint32_t addr, FILE* out);

void fprint_U(const Instruction& i, uint32_t addr, FILE* out);

void fprint_I(const Instruction& i, uint32_t addr, FILE* out);

void fprint_Il(const Instruction& i, uint32_t addr, FILE* out);

void fprint_unknown(const Instruction& i, uint32_t addr, FILE* out);

void fprint_command(const Instruction& i, uint32_t addr, FILE* out);

#endif //RISC_V_DISASSEMBLER_PRINTING_H
