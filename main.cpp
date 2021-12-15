#include <iostream>
#include <vector>
#include <map>
#include "fstream"
#include "elf_parsing.h"
#include "instructions.h"


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

struct command {
    command(uint16_t b1, uint16_t b2) {

    }
    std::string name;
    uint8_t opcode;
    uint8_t funct3;
    uint8_t rd;
};

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "2 program arguments expected\n";
        return 1;
    }
    auto in = std::ifstream(argv[1]);
    if (!in.is_open()) {
        std::cout << "Unable to open file " << argv[1] << "\n";
        return 1;
    }
    Elf_header header{};
    read_elf_header(header, in);

    //Read string table
    Elf_section str_table{};
    in.seekg(header.e_shoff, std::ios::beg);
    for (int i = 0; i <= header.e_shstrndx && !in.eof(); i++) {
        read_elf_section(str_table, in);
    }
    in.seekg(str_table.sh_offset, std::ios::beg);
    unsigned int names_size = str_table.sh_size;
    char* section_names = new char[names_size];
    in.read(section_names, names_size);



   //Read symtable
    in.seekg(header.e_shoff, std::ios::beg);
    Elf_section symtab{};
    while (symtab.sh_type != 2 && !in.eof()) {
        read_elf_section(symtab, in);
    }
    in.seekg(symtab.sh_offset, std::ios::beg);
    unsigned int entry_count = symtab.sh_size / symtab.sh_entsize;

    //Parse each entry and store names in a map
    std::vector<Symtab_entry> entries(entry_count);
    for (int i = 0; i < entry_count; i++) {
        read_symtab_entry(entries[i], in);
        entries[i].index = i;
        if (entries[i].st_name != 0)
            entries[i].name = std::string(section_names + entries[i].st_name);
    }
    std::map<uint32_t, std::string> labels;
    for (auto entry: entries) {
        labels[entry.st_value] = entry.name;
    }

    //Parse .text
    in.seekg(header.e_shoff, std::ios::beg);
    Elf_section text{};

    for (int i = 0; i < header.e_shnum; i++) {
        read_elf_section(text, in);
        if (std::string(section_names + text.sh_name) == ".text") break;
    }
    in.seekg(text.sh_offset, std::ios::beg);
    FILE* out = fopen(argv[2], "w");

    fprintf(out, ".text\n");

    uint32_t addr = 0;
    while (addr < text.sh_size) {
        uint16_t b1;
        in.read((char*) &b1, sizeof(b1));
        if ((b1 & 3) == 3) {
            uint16_t b2;
            in.read((char*) &b2, sizeof(b2));
            Instruction i = parse_instruction(b1, b2);
            if (labels.find(addr) != labels.end())
                i.label = labels[addr];
            fprint_command(i, addr, out);
            addr+=4;
        }
        else {
            std::cout << "RVC instructions not supported - exiting\n";
            break;
        }
    }
    fprintf(out, "\n");
    fprint_symtab(entries, out);

    in.close();
    fclose(out);
    delete[] section_names;
    return 0;
}
