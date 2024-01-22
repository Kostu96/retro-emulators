#include "bios.h"

#include <fstream>
#include <iostream>

BIOS::BIOS()
{
    std::ifstream fin(FILENAME, std::ios::binary | std::ios::ate);
    if (!fin.is_open()) {
        std::cerr << "Failed to open file: " << FILENAME << '\n';
        abort();
    }

    std::streamsize size = fin.tellg();
    if (size != SIZE) {
        std::cerr << "BIOS file should have 512KB. Size read is " << size << '\n';
        abort();
    }

    m_data = new uint8_t[size];
    fin.seekg(0);
    if (!fin.read((char*)m_data, size)) {
        std::cerr << "Failed to read from: " << FILENAME << "\nFile size was: " << size << '\n';
        abort();
    }

    fin.close();
}

BIOS::~BIOS()
{
    delete[] m_data;
}

uint8_t BIOS::load8(uint32_t offset) const
{
    return m_data[offset];
}

uint32_t BIOS::load32(uint32_t offset) const
{
    uint8_t b0 = m_data[offset];
    uint8_t b1 = m_data[offset + 1];
    uint8_t b2 = m_data[offset + 2];
    uint8_t b3 = m_data[offset + 3];

    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

const char* BIOS::FILENAME = "assets/SCPH-1001.bin";
