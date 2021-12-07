#include "iostream"
#include "fstream"

#ifndef RISC_LAB_BITIO_H
#define RISC_LAB_BITIO_H

const int byte_size = 8;

class BitWriter {
public:
    explicit BitWriter(std::ofstream& out) : out(out) {};
    void flush() {
        if (byte_index > 0) {
            out.write((char *) &byte, sizeof(char));
        }
    }

    template<class T>
    friend BitWriter& operator<<(BitWriter& w, T& var) {
        w.flush();
        w.out.write((char*) &var, sizeof(var));
        return w;
    }

    friend BitWriter& operator<<(BitWriter& w, bool& b) {
        if (!b)
            w.byte &= ~(1 << (byte_size - 1 - w.byte_index));
        else
            w.byte |= (1 << (byte_size - 1 - w.byte_index));
        w.byte_index++;
        if (w.byte_index == byte_size) {
            w.out.write((char*) &w.byte, sizeof(char));
            w.byte_index = 0;
            w.byte = 0;
        }
        return w;
    }

private:
    unsigned char byte = 0;
    int byte_index = 0;
    std::ofstream& out;
};

class BitReader {
public:
    explicit BitReader(std::ifstream& in) : in(in) {}

    template<class T>
    friend BitReader& operator>>(BitReader& r, T& var) {
        r.in.read((char*) &var, sizeof(var));
        return r;
    }

    friend BitReader& operator>>(BitReader& r, bool& b) {
        if (r.byte_index == 0) r >> r.byte;
        b = r.byte & (1 << (byte_size - 1 - r.byte_index++));
        if (r.byte_index == byte_size) {
            r.byte_index = 0;
        }
        return r;
    }

    operator bool() const {
        return in.operator bool();
    }


private:
    unsigned char byte = 0;
    int byte_index = 0;
    std::ifstream& in;
};


#endif //RISC_LAB_BITIO_H
