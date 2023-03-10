#include "cpu_x80_base.inl"

#include <bitset>

template <>
void CPUx80<CPUx80Mode::Intel8080>::clock()
{
    m_cyclesLeft = 0; // TODO: temp WA until i8080 cycle times are implemented

    if (m_cyclesLeft > 0)
        m_cyclesLeft--;

    if (m_EIRequested) {
        m_EIRequested = false;
        m_state.InterruptEnabled = true;
    }

    if (m_cyclesLeft == 0)
    {
        if (m_interruptRequested) {
            m_interruptRequested = false;
            RST(m_interruptVector);
        }

        if (!m_state.IsHalted) {
            u8 opcode = load8(m_state.PC++);

            standardInstruction(opcode);
            // m_cyclesLeft += m_conditionalTaken ? conditionalCycleCounts[opcode] : standardCycleCounts[opcode];
            m_conditionalTaken = false;
        }
    }
}

template <>
u8 CPUx80<CPUx80Mode::Intel8080>::getCarryFlag()
{
    return m_state.F.i8080.Carry;
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::setCarryFlag(u8 value)
{
    m_state.F.i8080.Carry = value;
}

template <>
u8 CPUx80<CPUx80Mode::Intel8080>::getHalfCarryFlag()
{
    return m_state.F.i8080.HalfCarry;
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::setHalfCarryFlag(u8 value)
{
    m_state.F.i8080.HalfCarry = value;
}

template <>
u8 CPUx80<CPUx80Mode::Intel8080>::getZeroFlag()
{
    return m_state.F.i8080.Zero;;
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::setZeroFlag(u8 value)
{
    m_state.F.i8080.Zero = value;
}

template <>
u8 CPUx80<CPUx80Mode::Intel8080>::getSignFlag()
{
    return m_state.F.i8080.Sign;
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::setSignFlag(u8 value)
{
     m_state.F.i8080.Sign = value;
}

template <>
u8 CPUx80<CPUx80Mode::Intel8080>::getParityFlag()
{
    return m_state.F.i8080.Parity;
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::setParityFlag(u8 value)
{
    m_state.F.i8080.Parity = value;
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::ADD(u8 value)
{
    u16 result;
    u8 result4bit;
    add8(value, 0, result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSignFlag(result >> 7);
    setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::ADC(u8 value)
{
    u16 result;
    u8 result4bit;
    add8(value, getCarryFlag(), result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSignFlag(result >> 7);
    setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::ADDHL(u16 value)
{
    u32 result;
    u16 result12bit;
    add16(value, result, result12bit);

    setCarryFlag(result >> 16);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::SUB(u8 value)
{
    u16 result;
    u8 result4bit;
    subtract(value, 0, result, result4bit);
    
    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSignFlag(result >> 7);
    setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::SBB(u8 value)
{
    u16 result;
    u8 result4bit;
    subtract(value, getCarryFlag(), result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSignFlag(result >> 7);
    setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::CMP(u8 value)
{
    u16 result;
    u8 result4bit;
    compare(value, result, result4bit);
    
    setZeroFlag(result == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSignFlag(result >> 7);
    setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::DAA()
{
    u16 value = m_state.A;

    if ((value & 0x0F) > 9 || getHalfCarryFlag())
        value += 0x06;

    if ((value >> 4) > 9 || getCarryFlag())
        value += 0x60;

    m_state.A = value;
    
    setCarryFlag(value >> 8);
    setParityFlag((std::bitset<8>(m_state.A).count() % 2) == 0);
    setSignFlag(m_state.A >> 7);
    setZeroFlag(m_state.A == 0);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::RLCA()
{
    u8 newCarry;
    rotateLeft(m_state.A, newCarry);

    setCarryFlag(newCarry);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::RAL()
{
    u8 newCarry;
    rotateLeftWithCarry(m_state.A, getCarryFlag(), newCarry);

    setCarryFlag(newCarry);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::RRCA()
{
    u8 newCarry;
    rotateRight(m_state.A, newCarry);

    setCarryFlag(newCarry);
}

template <>
void CPUx80<CPUx80Mode::Intel8080>::RAR()
{
    u8 newCarry;
    rotateRightWithCarry(m_state.A, getCarryFlag(), newCarry);

    setCarryFlag(newCarry);
}

template class CPUx80<CPUx80Mode::Intel8080>;
