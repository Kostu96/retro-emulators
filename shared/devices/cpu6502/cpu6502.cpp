#include "cpu6502.hpp"

#include <cassert>

#ifdef DEBUG_LOG_ENABLE
#include <iostream>
#define DEBUG_LOG(x) std::cout << x << '\n'
#else
#define DEBUG_LOG(x)
#endif

void CPU6502::reset()
{
    m_cyclesLeft = 0;

    PC = load16(0xFFFC); // RESET vector
    ACC = X = Y = 0;
    SP = 0xFD;
    F.bits.U = 1;
    F.bits.I = 1;

    m_cyclesLeft += 6;
}

void CPU6502::clock()
{
    if (m_cyclesLeft == 0)
    {
        if (m_irq && F.bits.I == 0) IRQ();
        if (m_nmi && !m_isDuringNMI) NMI();

        u8 instruction = load8(PC++);
        m_cyclesLeft++;

        switch (instruction)
        {
        case 0x00:           op_BRK(); break;
        case 0x01: am_INX(); op_ORA(); break;
        case 0x05: am_ZPG(); op_ORA(); break;
        case 0x06: am_ZPG(); op_ASL(); break;
        case 0x08:           op_PHP(); break;
        case 0x09: am_IMM(); op_ORA(); break;
        case 0x0A: am_ACC(); op_ASL(); break;
        case 0x0D: am_ABS(); op_ORA(); break;
        case 0x0E: am_ABS(); op_ASL(); break;
        case 0x10:           op_BPL(); break;
        case 0x11: am_INY(); op_ORA(); break;
        case 0x15: am_ZPX(); op_ORA(); break;
        case 0x16: am_ZPX(); op_ASL(); break;
        case 0x18:           op_CLC(); break;
        case 0x19: am_ABY(); op_ORA(); break;
        case 0x1D: am_ABX(); op_ORA(); break;
        case 0x1E: am_ABX(); op_ASL(); break;
        case 0x20: am_ABS(); op_JSR(); break;
        case 0x21: am_INX(); op_AND(); break;
        case 0x24: am_ZPG(); op_BIT(); break;
        case 0x25: am_ZPG(); op_AND(); break;
        case 0x26: am_ZPG(); op_ROL(); break;
        case 0x28:           op_PLP(); break;
        case 0x29: am_IMM(); op_AND(); break;
        case 0x2A: am_ACC(); op_ROL(); break;
        case 0x2C: am_ABS(); op_BIT(); break;
        case 0x2D: am_ABS(); op_AND(); break;
        case 0x2E: am_ABS(); op_ROL(); break;
        case 0x30:           op_BMI(); break;
        case 0x31: am_INY(); op_AND(); break;
        case 0x35: am_ZPX(); op_AND(); break;
        case 0x36: am_ZPX(); op_ROL(); break;
        case 0x38:           op_SEC(); break;
        case 0x39: am_ABY(); op_AND(); break;
        case 0x3D: am_ABX(); op_AND(); break;
        case 0x3E: am_ABX(); op_ROL(); break;
        case 0x40:           op_RTI(); break;
        case 0x41: am_INX(); op_EOR(); break;
        case 0x45: am_ZPG(); op_EOR(); break;
        case 0x46: am_ZPG(); op_LSR(); break;
        case 0x48:           op_PHA(); break;
        case 0x49: am_IMM(); op_EOR(); break;
        case 0x4A: am_ACC(); op_LSR(); break;
        case 0x4C: am_ABS(); op_JMP(); break;
        case 0x4D: am_ABS(); op_EOR(); break;
        case 0x4E: am_ABS(); op_LSR(); break;
        case 0x50:           op_BVC(); break;
        case 0x51: am_INY(); op_EOR(); break;
        case 0x55: am_ZPX(); op_EOR(); break;
        case 0x56: am_ZPX(); op_LSR(); break;
        case 0x58:           op_CLI(); break;
        case 0x59: am_ABY(); op_EOR(); break;
        case 0x5D: am_ABX(); op_EOR(); break;
        case 0x5E: am_ABX(); op_LSR(); break;
        case 0x60:           op_RTS(); break;
        case 0x61: am_INX(); op_ADC(); break;
        case 0x65: am_ZPG(); op_ADC(); break;
        case 0x66: am_ZPG(); op_ROR(); break;
        case 0x68:           op_PLA(); break;
        case 0x69: am_IMM(); op_ADC(); break;
        case 0x6A: am_ACC(); op_ROR(); break;
        case 0x6C: am_IND(); op_JMP(); break;
        case 0x6D: am_ABS(); op_ADC(); break;
        case 0x6E: am_ABS(); op_ROR(); break;
        case 0x70:           op_BVS(); break;
        case 0x71: am_INY(); op_ADC(); break;
        case 0x75: am_ZPX(); op_ADC(); break;
        case 0x76: am_ZPX(); op_ROR(); break;
        case 0x78:           op_SEI(); break;
        case 0x79: am_ABY(); op_ADC(); break;
        case 0x7D: am_ABX(); op_ADC(); break;
        case 0x7E: am_ABX(); op_ROR(); break;
        case 0x81: am_INX(); op_STA(); break;
        case 0x84: am_ZPG(); op_STY(); break;
        case 0x85: am_ZPG(); op_STA(); break;
        case 0x86: am_ZPG(); op_STX(); break;
        case 0x88:           op_DEY(); break;
        case 0x8A:           op_TXA(); break;
        case 0x8C: am_ABS(); op_STY(); break;
        case 0x8D: am_ABS(); op_STA(); break;
        case 0x8E: am_ABS(); op_STX(); break;
        case 0x90:           op_BCC(); break;
        case 0x91: am_INY(); op_STA(); break;
        case 0x94: am_ZPX(); op_STY(); break;
        case 0x95: am_ZPX(); op_STA(); break;
        case 0x96: am_ZPY(); op_STX(); break;
        case 0x98:           op_TYA(); break;
        case 0x99: am_ABY(); op_STA(); break;
        case 0x9A:           op_TXS(); break;
        case 0x9D: am_ABX(); op_STA(); break;
        case 0xA0: am_IMM(); op_LDY(); break;
        case 0xA1: am_INX(); op_LDA(); break;
        case 0xA2: am_IMM(); op_LDX(); break;
        case 0xA4: am_ZPG(); op_LDY(); break;
        case 0xA5: am_ZPG(); op_LDA(); break;
        case 0xA6: am_ZPG(); op_LDX(); break;
        case 0xA8:           op_TAY(); break;
        case 0xA9: am_IMM(); op_LDA(); break;
        case 0xAA:           op_TAX(); break;
        case 0xAC: am_ABS(); op_LDY(); break;
        case 0xAD: am_ABS(); op_LDA(); break;
        case 0xAE: am_ABS(); op_LDX(); break;
        case 0xB0:           op_BCS(); break;
        case 0xB1: am_INY(); op_LDA(); break;
        case 0xB4: am_ZPX(); op_LDY(); break;
        case 0xB5: am_ZPX(); op_LDA(); break;
        case 0xB6: am_ZPY(); op_LDX(); break;
        case 0xB8:           op_CLV(); break;
        case 0xB9: am_ABY(); op_LDA(); break;
        case 0xBA:           op_TSX(); break;
        case 0xBC: am_ABX(); op_LDY(); break;
        case 0xBD: am_ABX(); op_LDA(); break;
        case 0xBE: am_ABY(); op_LDX(); break;
        case 0xC0: am_IMM(); op_CPY(); break;
        case 0xC1: am_INX(); op_CMP(); break;
        case 0xC4: am_ZPG(); op_CPY(); break;
        case 0xC5: am_ZPG(); op_CMP(); break;
        case 0xC6: am_ZPG(); op_DEC(); break;
        case 0xC8:           op_INY(); break;
        case 0xCA:           op_DEX(); break;
        case 0xCC: am_ABS(); op_CPY(); break;
        case 0xCD: am_ABS(); op_CMP(); break;
        case 0xC9: am_IMM(); op_CMP(); break;
        case 0xCE: am_ABS(); op_DEC(); break;
        case 0xD0:           op_BNE(); break;
        case 0xD1: am_INY(); op_CMP(); break;
        case 0xD5: am_ZPX(); op_CMP(); break;
        case 0xD6: am_ZPX(); op_DEC(); break;
        case 0xD8:           op_CLD(); break;
        case 0xD9: am_ABY(); op_CMP(); break;
        case 0xDD: am_ABX(); op_CMP(); break;
        case 0xDE: am_ABX(); op_DEC(); break;
        case 0xE0: am_IMM(); op_CPX(); break;
        case 0xE1: am_INX(); op_SBC(); break;
        case 0xE4: am_ZPG(); op_CPX(); break;
        case 0xE5: am_ZPG(); op_SBC(); break;
        case 0xE6: am_ZPG(); op_INC(); break;
        case 0xE8:           op_INX(); break;
        case 0xE9: am_IMM(); op_SBC(); break;
        case 0xEA:           op_NOP(); break;
        case 0xEC: am_ABS(); op_CPX(); break;
        case 0xED: am_ABS(); op_SBC(); break;
        case 0xEE: am_ABS(); op_INC(); break;
        case 0xF0:           op_BEQ(); break;
        case 0xF1: am_INY(); op_SBC(); break;
        case 0xF5: am_ZPX(); op_SBC(); break;
        case 0xF6: am_ZPX(); op_INC(); break;
        case 0xF8:           op_SED(); break;
        case 0xF9: am_ABY(); op_SBC(); break;
        case 0xFD: am_ABX(); op_SBC(); break;
        case 0xFE: am_ABX(); op_INC(); break;
        default:
            assert(false && "Unhandled instruction");
        }
    }

    m_cyclesLeft--;
}

void CPU6502::runUntilEndlessLoop()
{
    u16 lastPC = PC;
    u8 counter = 0;
    while (counter < 10)
    {
        clock();

        if (lastPC == PC)
            counter++;
        else
            counter = 0;

        lastPC = PC;
    }
}

void CPU6502::IRQ()
{
    push16(PC);

    F.bits.B = 0;
    push8(F.byte);
    F.bits.I = 1;

    PC = load16(0xFFFE); // IRQ vector

    m_cyclesLeft += 2;
}

void CPU6502::NMI()
{
    m_isDuringNMI = true;
    push16(PC);

    F.bits.B = 0;
    push8(F.byte);
    F.bits.I = 1;

    PC = load16(0xFFFA); // NMI vector

    m_cyclesLeft += 3;
}

#pragma region AdressingModes
void CPU6502::am_ACC()
{
    m_isACCAddressing = true;
}

void CPU6502::am_IMM()
{
    m_absoluteAddress = PC++;
}

void CPU6502::am_ZPG()
{
    m_absoluteAddress = load8(PC++);
}

void CPU6502::am_ZPX()
{
    m_absoluteAddress = load8(PC++) + X;
    m_absoluteAddress &= 0x00FF;
    m_cyclesLeft++;
}

void CPU6502::am_ZPY()
{
    m_absoluteAddress = load8(PC++) + Y;
    m_absoluteAddress &= 0x00FF;
    m_cyclesLeft++;
}

void CPU6502::am_ABS()
{
    m_absoluteAddress = load16(PC);
    PC += 2;
}

void CPU6502::am_ABX()
{
    m_absoluteAddress = load16(PC) + X;
    PC += 2;
    m_cyclesLeft++;
}

void CPU6502::am_ABY()
{
    m_absoluteAddress = load16(PC) + Y;
    PC += 2;
    m_cyclesLeft++;
}

void CPU6502::am_IND()
{
    u16 ptr = load16(PC);
    PC += 2;
    m_absoluteAddress = load8(((ptr & 0xFF) == 0xFF) ? ptr & 0xFF00 : ptr + 1) << 8;
    m_absoluteAddress |= load8(ptr);

}

void CPU6502::am_INX()
{
    u8 ptr = load8(PC++) + X;
    m_absoluteAddress = load8(ptr + 1) << 8;
    m_absoluteAddress |= load8(ptr);
    m_cyclesLeft++;
}

void CPU6502::am_INY()
{
    u8 ptr = load8(PC++);
    m_absoluteAddress = load16(ptr) + Y;
}
#pragma endregion

void CPU6502::op_NOP()
{
    DEBUG_LOG("NOP");

    m_cyclesLeft++;
}

#pragma region BranchInstructions
void CPU6502::op_JMP()
{
    DEBUG_LOG("JMP " << std::hex << m_absoluteAddress);

    PC = m_absoluteAddress;
    m_cyclesLeft++;
}

void CPU6502::op_BPL()
{
    s8 offset = load8(PC++);

    DEBUG_LOG("BPL " << std::hex << offset);

    if (F.bits.N == 0)
        PC += offset;
}

void CPU6502::op_BMI()
{
    s8 offset = load8(PC++);

    DEBUG_LOG("BMI " << std::hex << offset);

    if (F.bits.N == 1)
        PC += offset;
}

void CPU6502::op_BEQ()
{
    s8 offset = load8(PC++);

    DEBUG_LOG("BEQ " << std::hex << offset);

    if (F.bits.Z == 1)
        PC += offset;
}

void CPU6502::op_BNE()
{
    s8 offset = load8(PC++);

    DEBUG_LOG("BNE " << std::hex << offset);

    if (F.bits.Z == 0)
        PC += offset;
}

void CPU6502::op_BCS()
{
    s8 offset = load8(PC++);

    DEBUG_LOG("BCS " << std::hex << offset);

    if (F.bits.C == 1)
        PC += offset;
}

void CPU6502::op_BCC()
{
    s8 offset = load8(PC++);

    DEBUG_LOG("BCC " << std::hex << offset);

    if (F.bits.C == 0)
        PC += offset;
}

void CPU6502::op_BVS()
{
    s8 offset = load8(PC++);

    DEBUG_LOG("BVS " << std::hex << offset);

    if (F.bits.V == 1)
        PC += offset;
}

void CPU6502::op_BVC()
{
    s8 offset = load8(PC++);

    DEBUG_LOG("BVC " << std::hex << offset);

    if (F.bits.V == 0)
        PC += offset;
}

void CPU6502::op_JSR()
{
    DEBUG_LOG("JSR " << std::hex << m_absoluteAddress);

    push16(PC - 1);
    PC = m_absoluteAddress;
}

void CPU6502::op_RTS()
{
    DEBUG_LOG("RTS");

    PC = pop16() + 1;
}

void CPU6502::op_BRK()
{
    DEBUG_LOG("BRK");

    push16(PC + 1);

    push8(F.byte | 0x10);
    F.bits.I = 1;

    PC = load16(0xFFFE); // IRQ vector

    m_cyclesLeft += 2;
}

void CPU6502::op_RTI()
{
    DEBUG_LOG("RTI");

    F.byte = pop8() & 0xEF;
    PC = pop16();

    if (m_isDuringNMI) m_isDuringNMI = false;
}
#pragma endregion

#pragma region LoadStoreInstructions
void CPU6502::op_LDA()
{
    DEBUG_LOG("LDA");

    ACC = load8(m_absoluteAddress);
    m_cyclesLeft++;
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_LDX()
{
    DEBUG_LOG("LDX");

    X = load8(m_absoluteAddress);
    m_cyclesLeft++;
    F.bits.Z = (X == 0);
    F.bits.N = (X >> 7);
}

void CPU6502::op_LDY()
{
    DEBUG_LOG("LDY");

    Y = load8(m_absoluteAddress);
    m_cyclesLeft++;
    F.bits.Z = (Y == 0);
    F.bits.N = (Y >> 7);
}

void CPU6502::op_STA()
{
    DEBUG_LOG("STA");

    store8(m_absoluteAddress, ACC);
    m_cyclesLeft++;
}

void CPU6502::op_STX()
{
    DEBUG_LOG("STX");

    store8(m_absoluteAddress, X);
    m_cyclesLeft++;
}

void CPU6502::op_STY()
{
    DEBUG_LOG("STY");

    store8(m_absoluteAddress, Y);
    m_cyclesLeft++;
}
#pragma endregion

#pragma region TransferInstructions
void CPU6502::op_TXA()
{
    DEBUG_LOG("TXA");

    ACC = X;
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_TAX()
{
    DEBUG_LOG("TAX");

    X = ACC;
    F.bits.Z = (X == 0);
    F.bits.N = (X >> 7);
}

void CPU6502::op_TYA()
{
    DEBUG_LOG("TYA");

    ACC = Y;
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_TAY()
{
    DEBUG_LOG("TAY");

    Y = ACC;
    F.bits.Z = (Y == 0);
    F.bits.N = (Y >> 7);
}

void CPU6502::op_TSX()
{
    DEBUG_LOG("TSX");

    X = SP;
    F.bits.Z = (X == 0);
    F.bits.N = (X >> 7);
}

void CPU6502::op_TXS()
{
    DEBUG_LOG("TXS");

    SP = X;
}
#pragma endregion

#pragma region StackInstructions
void CPU6502::op_PHA()
{
    push8(ACC);
}

void CPU6502::op_PLA()
{
    ACC = pop8();
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_PHP()
{
    push8(F.byte | 0x10);
}

void CPU6502::op_PLP()
{
    F.byte = pop8() | 0x20;
}
#pragma endregion

#pragma region ALUInstructions
void CPU6502::op_ADC()
{
    u16 temp;
    u16 memory = load8(m_absoluteAddress);
    
    if (F.bits.D)
    {
        u8 memL = memory & 0x0F;
        u8 memH = memory & 0xF0;
        u8 accL = ACC & 0x0F;
        u8 accH = ACC & 0xF0;

        u8 YL = accL + memL + F.bits.C;
        if (YL > 0x09) YL += 0x06;
        temp = accH + memH + (YL & 0xF0);
        if (temp > 0x90) temp += 0x60;
        temp |= (YL & 0x0F);
    }
    else
    {
        temp = ACC + memory + F.bits.C;
    }

    F.bits.V = (~(static_cast<u16>(ACC) ^ memory) & (static_cast<u16>(ACC) ^ temp)) >> 7;
    ACC = temp & 0xFF;

    F.bits.C = (temp >> 8);
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_SBC()
{
    u16 temp;
    u16 memory = load8(m_absoluteAddress);

    if (F.bits.D)
    {
        u8 memL = 0x09 - (memory & 0x0F);
        u8 memH = 0x90 - (memory & 0xF0);
        u8 accL = ACC & 0x0F;
        u8 accH = ACC & 0xF0;

        u8 YL = accL + memL + F.bits.C;
        if (YL > 0x09) YL += 0x06;
        temp = accH + memH + (YL & 0xF0);
        if (temp > 0x90) temp += 0x60;
        temp |= (YL & 0x0F);
    }
    else
    {
        memory ^= 0x00FF;
        temp = ACC + memory + F.bits.C;
    }
    
    F.bits.V = (~(static_cast<u16>(ACC) ^ memory) & (static_cast<u16>(ACC) ^ temp)) >> 7;
    ACC = temp & 0xFF;

    F.bits.C = (temp >> 8);
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_ORA()
{
    ACC |= load8(m_absoluteAddress);
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_AND()
{
    ACC &= load8(m_absoluteAddress);
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_EOR()
{
    ACC ^= load8(m_absoluteAddress);
    F.bits.Z = (ACC == 0);
    F.bits.N = (ACC >> 7);
}

void CPU6502::op_LSR()
{
    u8 data;
    if (m_isACCAddressing)
    {
        F.bits.C = (ACC & 1);
        ACC >>= 1;
        ACC &= 0x7F;
        
        m_isACCAddressing = false;
        data = ACC;
    }
    else
    {
        data = load8(m_absoluteAddress);
        F.bits.C = (data & 1);
        data >>= 1;
        data &= 0x7F;
        store8(m_absoluteAddress, data);
    }

    F.bits.Z = (data == 0);
    F.bits.N = 0;
}

void CPU6502::op_ASL()
{
    u8 data;
    if (m_isACCAddressing)
    {
        F.bits.C = (ACC >> 7);
        ACC <<= 1;

        m_isACCAddressing = false;
        data = ACC;
    }
    else
    {
        data = load8(m_absoluteAddress);
        F.bits.C = (data >> 7);
        data <<= 1;
        store8(m_absoluteAddress, data);
    }

    F.bits.Z = (data == 0);
    F.bits.N = (data >> 7);
}

void CPU6502::op_ROL()
{
    u8 data;
    u8 tempCarry = F.bits.C;
    if (m_isACCAddressing)
    {
        F.bits.C = (ACC >> 7);
        ACC <<= 1;
        ACC |= tempCarry;

        m_isACCAddressing = false;
        data = ACC;
    }
    else
    {
        data = load8(m_absoluteAddress);
        F.bits.C = (data >> 7);
        data <<= 1;
        data |= tempCarry;
        store8(m_absoluteAddress, data);
    }

    F.bits.Z = (data == 0);
    F.bits.N = (data >> 7);
}

void CPU6502::op_ROR()
{
    u8 data;
    u8 tempCarry = F.bits.C;
    tempCarry <<= 7;
    if (m_isACCAddressing)
    {
        F.bits.C = (ACC & 1);
        ACC >>= 1;
        ACC &= 0x7F;
        ACC |= tempCarry;

        m_isACCAddressing = false;
        data = ACC;
    }
    else
    {
        data = load8(m_absoluteAddress);
        F.bits.C = (data & 1);
        data >>= 1;
        data &= 0x7F;
        data |= tempCarry;
        store8(m_absoluteAddress, data);
    }

    F.bits.Z = (data == 0);
    F.bits.N = (data >> 7);
}

void CPU6502::op_BIT()
{
    u8 temp = load8(m_absoluteAddress);
    F.bits.Z = ((ACC & temp) == 0);
    F.bits.N = (temp >> 7);
    F.bits.V = (temp >> 6) & 1;
}

void CPU6502::op_INC()
{
    u8 data = load8(m_absoluteAddress);
    data++;
    store8(m_absoluteAddress, data);
    F.bits.Z = (data == 0);
    F.bits.N = (data >> 7);
}

void CPU6502::op_INX()
{
    X++;
    F.bits.Z = (X == 0);
    F.bits.N = (X >> 7);
}

void CPU6502::op_INY()
{
    Y++;
    F.bits.Z = (Y == 0);
    F.bits.N = (Y >> 7);
}

void CPU6502::op_DEC()
{
    u8 data = load8(m_absoluteAddress);
    data--;
    store8(m_absoluteAddress, data);
    F.bits.Z = (data == 0);
    F.bits.N = (data >> 7);
}

void CPU6502::op_DEX()
{
    X--;
    F.bits.Z = (X == 0);
    F.bits.N = (X >> 7);
}

void CPU6502::op_DEY()
{
    Y--;
    F.bits.Z = (Y == 0);
    F.bits.N = (Y >> 7);
}

void CPU6502::op_CMP()
{
    u16 memory = load8(m_absoluteAddress);
    u16 temp = static_cast<u16>(ACC) - memory;
    F.bits.C = ACC >= memory ? 1 : 0;
    F.bits.Z = ((temp & 0xFF) == 0);
    F.bits.N = ((temp & 0xFF) >> 7);
}

void CPU6502::op_CPX()
{
    u16 memory = load8(m_absoluteAddress);
    u16 temp = static_cast<u16>(X) - memory;
    F.bits.C = X >= memory ? 1 : 0;
    F.bits.Z = ((temp & 0xFF) == 0);
    F.bits.N = ((temp & 0xFF) >> 7);
}

void CPU6502::op_CPY()
{
    u16 memory = load8(m_absoluteAddress);
    u16 temp = static_cast<u16>(Y) - memory;
    F.bits.C = Y >= memory ? 1 : 0;
    F.bits.Z = ((temp & 0xFF) == 0);
    F.bits.N = ((temp & 0xFF) >> 7);
}
#pragma endregion

#pragma region FlagsInstructions
void CPU6502::op_SEC()
{
    F.bits.C = 1;
    m_cyclesLeft++;
}

void CPU6502::op_CLC()
{
    F.bits.C = 0;
    m_cyclesLeft++;
}

void CPU6502::op_SEI()
{
    F.bits.I = 1;
    m_cyclesLeft++;
}

void CPU6502::op_CLI()
{
    F.bits.I = 0;
    m_cyclesLeft++;
}

void CPU6502::op_SED()
{
    F.bits.D = 1;
    m_cyclesLeft++;
}

void CPU6502::op_CLD()
{
    F.bits.D = 0;
    m_cyclesLeft++;
}

void CPU6502::op_CLV()
{
    F.bits.V = 0;
    m_cyclesLeft++;
}
#pragma endregion
