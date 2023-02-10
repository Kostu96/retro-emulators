#pragma once
#include "../shared/type_aliases.hpp"

class BitFIFO
{
public:
	BitFIFO() = default;

	bool push(u8 byteL, u8 byteH);
	u8 pop();
	void clear();
private:
	u16 m_bufferH = 0;
	u16 m_bufferL = 0;
	u8 m_size = 0;
};
