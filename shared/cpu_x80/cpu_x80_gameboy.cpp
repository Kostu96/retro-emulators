#include "cpu_x80.inl"

template <>
u8 CPUx80<CPUx80Mode::GameBoy>::getCarryFlag()
{
    return m_state.F.gb.Carry;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::setCarryFlag(u8 value)
{
    m_state.F.gb.Carry = value;
}

template <>
u8 CPUx80<CPUx80Mode::GameBoy>::getHalfCarryFlag()
{
    return m_state.F.gb.HalfCarry;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::setHalfCarryFlag(u8 value)
{
    m_state.F.gb.HalfCarry = value;
}

template <>
u8 CPUx80<CPUx80Mode::GameBoy>::getSubtractFlag()
{
    return m_state.F.gb.Subtract;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::setSubtractFlag(u8 value)
{
    m_state.F.gb.Subtract = value;
}

template <>
u8 CPUx80<CPUx80Mode::GameBoy>::getZeroFlag()
{
    return m_state.F.gb.Zero;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::setZeroFlag(u8 value)
{
    m_state.F.gb.Zero = value;
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::ADD(u8 value)
{
    u16 result;
    u8 result4bit;
    add8(value, 0, result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::ADC(u8 value)
{
    u16 result;
    u8 result4bit;
    add8(value, getCarryFlag(), result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::ADDHL(u16 value)
{
    u32 result;
    u16 result12bit;
    add16(value, result, result12bit);
    
    setCarryFlag(result >> 16);
    setHalfCarryFlag(result12bit >> 12);
    setSubtractFlag(0);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SUB(u8 value)
{
    u16 result;
    u8 result4bit;
    subtract(value, 0, result, result4bit);
    
    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(1);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::SBB(u8 value)
{
    u16 result;
    u8 result4bit;
    subtract(value, getCarryFlag(), result, result4bit);

    setZeroFlag(m_state.A == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(1);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::CMP(u8 value)
{
    u16 result;
    u8 result4bit;
    compare(value, result, result4bit);
    
    setZeroFlag(result == 0);
    setHalfCarryFlag(result4bit >> 4);
    setCarryFlag(result >> 8);
    setSubtractFlag(1);
}

template <>
void CPUx80<CPUx80Mode::GameBoy>::DAA()
{
    u8 u = 0;
    u8 fc = 0;

    if (getHalfCarryFlag() || (!getSubtractFlag() && (m_state.A & 0xF) > 9)) {
        u = 6;
    }
    if (getCarryFlag() || (!getSubtractFlag() && m_state.A > 0x99)) {
        u |= 0x60;
        fc = 1;
    }

    m_state.A += getSubtractFlag() ? -u : u;

    setCarryFlag(fc);
    setHalfCarryFlag(0);
    setZeroFlag(m_state.A == 0);
}

template class CPUx80<CPUx80Mode::GameBoy>;
