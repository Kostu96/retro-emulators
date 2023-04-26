#include "apple2.hpp"

static constexpr u16 TEXT_GRAPHICS_SWITCH_ON_ADDRESS = 0xC050;
static constexpr u16 TEXT_GRAPHICS_SWITCH_OFF_ADDRESS = 0xC051;
static constexpr u16 MIXED_MODE_SWITCH_ON_ADDRESS = 0xC052;
static constexpr u16 MIXED_MODE_SWITCH_OFF_ADDRESS = 0xC053;
static constexpr u16 PAGE_SWITCH_ON_ADDRESS = 0xC054;
static constexpr u16 PAGE_SWITCH_OFF_ADDRESS = 0xC055;
static constexpr u16 GRAPHICS_MODE_SWITCH_ON_ADDRESS = 0xC056;
static constexpr u16 GRAPHICS_MODE_SWITCH_OFF_ADDRESS = 0xC057;

Apple2::Apple2()
{
    m_cpu.mapReadMemoryCallback([this](u16 address) { return memoryRead(address); });
    m_cpu.mapWriteMemoryCallback([this](u16 address, u8 data) { return memoryWrite(address, data); });

    m_cpu.reset();
}

void Apple2::update()
{
    m_cpu.clock();
}

u8 Apple2::memoryRead(u16 address)
{
    if (handleSwitches(address))
        return 0;

    return u8();
}

void Apple2::memoryWrite(u16 address, u8 /*data*/)
{
    if (handleSwitches(address))
        return;
}

bool Apple2::handleSwitches(u16 address)
{
    switch (address)
    {
    case TEXT_GRAPHICS_SWITCH_ON_ADDRESS:  m_textGraphicsSwitch = true;  break;
    case TEXT_GRAPHICS_SWITCH_OFF_ADDRESS: m_textGraphicsSwitch = false; break;
    case MIXED_MODE_SWITCH_ON_ADDRESS:     m_mixedModeSwitch = true;     break;
    case MIXED_MODE_SWITCH_OFF_ADDRESS:    m_mixedModeSwitch = false;    break;
    case PAGE_SWITCH_ON_ADDRESS:           m_pageSwitch = true;          break;
    case PAGE_SWITCH_OFF_ADDRESS:          m_pageSwitch = false;         break;
    case GRAPHICS_MODE_SWITCH_ON_ADDRESS:  m_graphicsModeSwitch = true;  break;
    case GRAPHICS_MODE_SWITCH_OFF_ADDRESS: m_graphicsModeSwitch = false; break;
    default:
        return false;
    }

    return true;
}
