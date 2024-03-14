#include "cpu.hpp"

namespace PSX {

	void CPU::reset()
	{
		m_PC = 0xBFC00000;
	}

	void CPU::clock()
	{

		m_PC += 4;
	}

} // namespace PSX
