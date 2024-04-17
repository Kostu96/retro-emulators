#include "chip8.hpp"
#include "disasm_chip8.hpp"
#include "chip8_instruction.hpp"

#include "shared/source/file_io.hpp"

#include <cassert>
#include <cstring>
#include <iostream>

void CHIP8::loadProgram(const char* filename)
{
    const void* PROGRAM_START = m_memory + 0x200;
    const size_t MAX_PROGRAM_SIZE = MEMORY_SIZE - 0x200;

    constexpr u8 CHARSET_SIZE = 80;
    const u8 charset[CHARSET_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    std::memcpy(m_memory, charset, CHARSET_SIZE);

    size_t size = MAX_PROGRAM_SIZE;
    bool ret = readFile(filename, (char*)PROGRAM_START, size, true);
    if (!ret) {
        std::cerr << "Could not read ROM file: " << filename << '\n';
    }

    disassemble((u8*)PROGRAM_START, size, m_disassembly);
}

void CHIP8::reset()
{
    std::srand(1234567890);

    std::memset(Screen, 0, 8 * 32);

    PC = 0x200;
    SP = 0;

    DT = ST = 0;

    for (u8 i = 0; i < 16; i++)
        keys[i] = false;

    m_elspsedTime = 0.0;
}

void CHIP8::update(double dt)
{
    m_elspsedTime += dt;

    if (m_elspsedTime > 16.67)
    {
        m_elspsedTime -= 16.67;
        if (DT) DT--;
        if (ST) ST--;
    }

    Instruction instruction;
    instruction.h2 = m_memory[PC++];
    instruction.h1 = m_memory[PC++];

    switch (instruction.n4)
    {
    case 0x0:
        if (instruction.word == 0x00E0)
        {
            std::memset(Screen, 0, 8 * 32);
            std::memset(m_screenPixels, 0, CHIP8_WIDTH * CHIP8_HEIGHT * sizeof(u32));
        }
        else if (instruction.word == 0x00EE)
        {
            PC = (Stack[SP - 2] << 8);
            PC |= Stack[SP - 1];
            SP -= 2;
        }
        else
            assert(false);
        break;
    case 0x1:
        PC = instruction.word & 0x0FFF;
        break;
    case 0x2:
        Stack[SP++] = PC >> 8;
        Stack[SP++] = PC & 0xFF;
        PC = instruction.word & 0x0FFF;
        break;
    case 0x3:
        if (GPR[instruction.n3] == instruction.h1)
            PC += 2;
        break;
    case 0x4:
        if (GPR[instruction.n3] != instruction.h1)
            PC += 2;
        break;
    case 0x5:
        if (GPR[instruction.n3] == GPR[instruction.n2])
            PC += 2;
        break;
    case 0x6:
        GPR[instruction.n3] = instruction.h1;
        break;
    case 0x7:
        GPR[instruction.n3] += instruction.h1;
        break;
    case 0x8:
        switch (instruction.n1)
        {
        case 0x0:
            GPR[instruction.n3] = GPR[instruction.n2];
            break;
        case 0x1:
            GPR[instruction.n3] |= GPR[instruction.n2];
            break;
        case 0x2:
            GPR[instruction.n3] &= GPR[instruction.n2];
            break;
        case 0x3:
            GPR[instruction.n3] ^= GPR[instruction.n2];
            break;
        case 0x4: {
            u16 temp = (u16)GPR[instruction.n3] + (u16)GPR[instruction.n2];
            GPR[0xF] = temp >> 8;
            GPR[instruction.n3] = temp & 0xFF;
        } break;
        case 0x5: {
            s16 temp = (s16)GPR[instruction.n3] - (s16)GPR[instruction.n2];
            GPR[instruction.n3] = temp & 0xFF;
            GPR[0xF] = temp < 0 ? 0 : 1;
        } break;
        case 0x6: {
            u8 temp = GPR[instruction.n3] & 1;
            GPR[instruction.n3] >>= 1;
            GPR[0xF] = temp;
        } break;
        case 0x7: {
            s16 temp = (s16)GPR[instruction.n2] - (s16)GPR[instruction.n3];
            GPR[instruction.n3] = temp & 0xFF;
            GPR[0xF] = temp < 0 ? 0 : 1;
        } break;
        case 0xE: {
            u8 temp = GPR[instruction.n3] >> 7;
            GPR[instruction.n3] <<= 1;
            GPR[0xF] = temp;
        } break;
        default:
            assert(false && "Unhandled 0x8xxx!");
        }
        break;
    case 0x9:
        if (GPR[instruction.n3] != GPR[instruction.n2])
            PC += 2;
        break;
    case 0xA:
        I = instruction.word & 0x0FFF;
        break;
    case 0xC:
        GPR[instruction.n3] = (std::rand() % 256) & instruction.h1;
        break;
    case 0xD: {
        u8 x = GPR[instruction.n3];
        u8 x_byte = x / 8;
        u8 x_bit = x % 8;
        u8 y = GPR[instruction.n2];
        u8 bytes = instruction.n1;
        GPR[0xF] = 0;
        for (u8 i = 0; i < bytes; i++)
        {
            u16 index = (y + i) * 8 + x_byte;
            GPR[0xF] = (Screen[index] & m_memory[I + i] >> x_bit) | (Screen[index + 1] & m_memory[I + i] << (8 - x_bit));
            Screen[index] ^= m_memory[I + i] >> x_bit;
            Screen[index + 1] ^= m_memory[I + i] << (8 - x_bit);

            for (u16 bit = 0; bit < 8; bit++)
            {
                bool on = Screen[index] & (1 << (7 - bit));
                u16 x_coord = x_byte * 8 + bit;
                u16 y_coord = y + i;
                m_screenPixels[y_coord * CHIP8_WIDTH + x_coord] = on ? 0xFFFFFFFF : 0;
                on = Screen[index + 1] & (1 << (7 - bit));
                x_coord = (x_byte + 1) * 8 + bit;
                m_screenPixels[y_coord * CHIP8_WIDTH + x_coord] = on ? 0xFFFFFFFF : 0;
            }
        }
    } break;
    case 0xE:
        if (instruction.h1 == 0x9E)
        {
            if (keys[GPR[instruction.n3]])
                PC += 2;
        }
        else if (instruction.h1 == 0xA1)
        {
            if (!keys[GPR[instruction.n3]])
                PC += 2;
        }
        break;
    case 0xF:
        switch (instruction.h1)
        {
        case 0x07:
            GPR[instruction.n3] = DT;
            break;
        case 0x0A: {
            bool found = false;
            for (u8 i = 0; i < 16; i++)
                if (keys[i]) {
                    found = true;
                    break;
                }

            if (!found)
                PC -= 2;
        } break;
        case 0x15:
            DT = GPR[instruction.n3];
            break;
        case 0x18:
            ST = GPR[instruction.n3];
            break;
        case 0x1E:
            I += GPR[instruction.n3];
            break;
        case 0x29:
            I = GPR[instruction.n3] * 5;
            break;
        case 0x33: {
            u8 n1 = GPR[instruction.n3] % 10;
            u8 n2 = (GPR[instruction.n3] / 10) % 10;
            u8 n3 = GPR[instruction.n3] / 100;
            m_memory[I] = n3;
            m_memory[I + 1] = n2;
            m_memory[I + 2] = n1;
        } break;
        case 0x55:
            for (u8 i = 0; i <= instruction.n3; i++)
                m_memory[I + i] = GPR[i];
            break;
        case 0x65:
            for (u8 i = 0; i <= instruction.n3; i++)
                GPR[i] = m_memory[I + i];
            break;
        default:
            assert(false && "Unhandled 0xFxxx!");
        }
        break;
    default:
        assert(false && "Unhandled opcode!");
    }
}

void CHIP8::handleKey(int key, int action)
{
    constexpr int KEY1 = 49;
    constexpr int KEY2 = 50;
    constexpr int KEY3 = 51;
    constexpr int KEY4 = 52;
    constexpr int KEYQ = 81;
    constexpr int KEYW = 87;
    constexpr int KEYE = 69;
    constexpr int KEYR = 82;
    constexpr int KEYA = 65;
    constexpr int KEYS = 83;
    constexpr int KEYD = 68;
    constexpr int KEYF = 70;
    constexpr int KEYZ = 90;
    constexpr int KEYX = 88;
    constexpr int KEYC = 67;
    constexpr int KEYV = 86;

    switch (key)
    {
    case KEY1: keys[0x1] = action; break;
    case KEY2: keys[0x2] = action; break;
    case KEY3: keys[0x3] = action; break;
    case KEY4: keys[0xC] = action; break;
    case KEYQ: keys[0x4] = action; break;
    case KEYW: keys[0x5] = action; break;
    case KEYE: keys[0x6] = action; break;
    case KEYR: keys[0xD] = action; break;
    case KEYA: keys[0x7] = action; break;
    case KEYS: keys[0x8] = action; break;
    case KEYD: keys[0x9] = action; break;
    case KEYF: keys[0xE] = action; break;
    case KEYZ: keys[0xA] = action; break;
    case KEYX: keys[0x0] = action; break;
    case KEYC: keys[0xB] = action; break;
    case KEYV: keys[0xF] = action; break;
    }
}
