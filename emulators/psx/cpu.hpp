#pragma once
#include <ccl/non_copyable.h>
#include <ccl/types.hpp>

#include <functional>

namespace PSX {

	class CPU :
		public ccl::NonCopyable
	{
	public:
		using ReadMemoryCallback = std::function<u8(u32)>;
		using WriteMemoryCallback = std::function<void(u32, u8)>;
		void mapReadMemoryCallback(ReadMemoryCallback callback) { load8 = callback; }
		void mapWriteMemoryCallback(WriteMemoryCallback callback) { store8 = callback; }

		void reset();
		void clock();

		CPU() = default;
	private:
		ReadMemoryCallback load8 = nullptr;
		WriteMemoryCallback store8 = nullptr;

		u32 m_PC;
	};

} // namespace PSX
