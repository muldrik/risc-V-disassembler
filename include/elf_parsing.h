#ifndef RISC_LAB_ELF_PARSING_H
#define RISC_LAB_ELF_PARSING_H
#include "cstdint"
#include "string"
#include "fstream"

struct Elf_header {
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct Elf_section {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
};

struct Symtab_entry {
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    uint8_t st_info;
    uint8_t st_other;
    uint16_t st_shndx;
    int index = 0;
    std::string name;
};

void read_elf_header(Elf_header& h, std::ifstream& in);

void read_elf_section(Elf_section& s, std::ifstream& in);

void read_symtab_entry(Symtab_entry& e, std::ifstream & in);

std::string symbind_to_string(uint8_t raw_info);

std::string symtype_to_string(uint8_t raw_info);

std::string symvis_to_string(uint8_t other);

std::string symindex_to_string(uint32_t index);
#endif //RISC_LAB_ELF_PARSING_H
