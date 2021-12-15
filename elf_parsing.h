#ifndef RISC_LAB_ELF_PARSING_H
#define RISC_LAB_ELF_PARSING_H

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

void read_elf_header(Elf_header& h, std::ifstream& in) {
    in.read((char*) &(h.e_ident), 16);
    in.read((char*) &h.e_type, sizeof(h.e_type));
    in.read((char*) &h.e_machine, sizeof(h.e_machine));
    in.read((char*) &h.e_version, sizeof(h.e_version));
    in.read((char*) &h.e_entry, sizeof(h.e_entry));
    in.read((char*) &h.e_phoff, sizeof(h.e_phoff));
    in.read((char*) &h.e_shoff, sizeof(h.e_shoff));
    in.read((char*) &h.e_flags, sizeof(h.e_flags));
    in.read((char*) &h.e_ehsize, sizeof(h.e_ehsize));
    in.read((char*) &h.e_phentsize, sizeof(h.e_phentsize));
    in.read((char*) &h.e_phnum, sizeof(h.e_phnum));
    in.read((char*) &h.e_shentsize, sizeof(h.e_shentsize));
    in.read((char*) &h.e_shnum, sizeof(h.e_shnum));
    in.read((char*) &h.e_shstrndx, sizeof(h.e_shstrndx));


}

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

void read_elf_section(Elf_section& s, std::ifstream& in) {
    in.read((char*) &s, sizeof(s));
};



void read_symtab_entry(Symtab_entry& e, std::ifstream & in) {
    in.read((char*) &(e.st_name), sizeof(e.st_name));
    in.read((char*) &(e.st_value), sizeof(e.st_value));
    in.read((char*) &(e.st_size), sizeof(e.st_size));
    in.read((char*) &(e.st_info), sizeof(e.st_info));
    in.read((char*) &(e.st_other), sizeof(e.st_other));
    in.read((char*) &(e.st_shndx), sizeof(e.st_shndx));
}

std::string symbind_to_string(uint8_t raw_info) {
    uint8_t info = (raw_info >> 4);
    switch (info) {
        case 0: return "LOCAL";
        case 1: return "GLOBAL";
        case 2: return "WEAK";
        case 10: return "LOOS";
        case 12: return "HIOS";
        case 13: return "LOPROC";
        case 15: return "HIPROC";
        default: return "UNKNOWN";
    }
}

std::string symtype_to_string(uint8_t raw_info) {
    uint8_t info = (raw_info & 0xf);
    switch (info) {
        case 0: return "NOTYPE";
        case 1: return "OBJECT";
        case 2: return "FUNC";
        case 3: return "SECTION";
        case 4: return "FILE";
        case 5: return "COMMON";
        case 6: return "TLS";
        case 10: return "LOOS";
        case 12: return "HIOS";
        case 13: return "LOPROC";
        case 15: return "HIPROC";
        default: return "UNKNOWN";
    }
}

std::string symvis_to_string(uint8_t other) {
    switch (other & (0x3)) {
        case 0: return "DEFAULT";
        case 1: return "INTERNAL";
        case 2: return "HIDDEN";
        case 3: return "PROTECTED";
        case 4: return "EXPORTED";
        case 5: return "SINGLETON";
        case 6: return "ELIMINATE";
        default: return "UNKNOWN";
    }
}

std::string symindex_to_string(uint32_t index) {
    switch (index) {
        case 0: return "UNDEF";
        case (0xff00): return "BEFORE";
        case (0xff01): return "AFTER";
        case (0xff1f): return "HIPROC";
        case (0xff20): return "LOOS";
        case (0xff3f): return "HIOS";
        case (0xfff1): return "ABS";
        case (0xfff2): return "COMMON";
        case (0xffff): return "XINDEX";
        default: return std::to_string(index);
    }
}
#endif //RISC_LAB_ELF_PARSING_H
