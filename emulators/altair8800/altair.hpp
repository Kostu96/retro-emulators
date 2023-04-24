#pragma once
#include "cpu8080.hpp"

#include <ccl/non_copyable.h>

class Altair :
	public ccl::NonCopyable
{
public:
	Altair();
	~Altair();

	void run() { m_isStopped = false; }
	void stop() { m_isStopped = true; }
	void reset();
	void update();

	//const std::vector<DisassemblyLine>& getDisassembly() const { return m_disassembly; }
	void setAddressLatch(u16 address) { m_addressLatch = address; }
	u16 getAddressLatch() const { return m_addressLatch; }
	u8 getDataLatch() const { return m_dataLatch; }
private:
	u8 memoryRead(u16 address);
	void memoryWrite(u16 address, u8 data);

	CPU8080 m_CPU;
	u8* m_RAM;
	bool m_isStopped = true;
	u16 m_addressLatch;
	u8 m_dataLatch;

	//std::vector<DisassemblyLine> m_disassembly;
};
