#pragma once
#include "cpu_x80.hpp"

#include <ccl/non_copyable.h>

class Altair :
	public ccl::NonCopyable
{
public:
	Altair();
	~Altair();

	void reset();
	void update();
private:
	u8 memoryRead(u16 address);
	void memoryWrite(u16 address, u8 data);

	CPUx80<CPUx80Mode::Intel8080> m_CPU;
	u8* m_RAM;
};
