#pragma once
#include "../../types.hpp"

class SID
{
public:
    SID() = default;

    u8 load8(u16 address) const;
    void store8(u16 address, u8 data);
private:
    u8 m_filter_mode_and_main_volume_control;
};
