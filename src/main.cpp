#include <iostream>
#include <vector>
#include <map>
#include "fstream"
#include "elf_parsing.h"
#include "instructions.h"
#include "printing.h"





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
