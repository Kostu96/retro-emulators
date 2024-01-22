#pragma once
#include <cstdint>

class DMA
{
public:
	enum class Port : uint32_t {
		MDECin = 0,
		MDECout = 1,
		GPU = 2,
		CDROM = 3,
		SPU = 4,
		PIO = 5,
		OTC = 6,
		Invalid = 0xDEADBEEF,
	};

	struct Channel {
		enum Direction { ToRAM = 0, FromRAM = 1 };
		enum Step { Increment = 0, Decrement = 1 };
		enum Sync { Manual = 0, Request = 1, LinkedList = 2 };

		uint32_t getTransferSize() const;
		void done();

		union {
			struct {
				uint32_t address    : 24; // 0-23
				uint32_t alwaysZero : 8;  // 24-31
			} fields;
			uint32_t word = 0;
		} baseAddress;
		union {
			struct {
				uint16_t blockSize;  // 0-15
				uint16_t blockCount; // 16-31
			} fields;
			uint32_t word = 0;
		} blockControl;
		union {
			struct {
				uint32_t direction   : 1; // 0 
				uint32_t step        : 1; // 1
				uint32_t alwayZero6  : 6; // 2-7
				uint32_t chopEnable  : 1; // 8
				uint32_t sync        : 2; // 9-10
				uint32_t alwaysZero5 : 5; // 11-15
				uint32_t chopDMASize : 3; // 16-18
				uint32_t alwaysZero4 : 1; // 19
				uint32_t chopCPUSize : 3; // 20-22
				uint32_t alwaysZero3 : 1; // 23
				uint32_t enable      : 1; // 24
				uint32_t alwaysZero2 : 3; // 25-27
				uint32_t trigger     : 1; // 28
				uint32_t unknown     : 2; // 29-30
				uint32_t alwaysZero1 : 1; // 31
			} fields;
			uint32_t word = 0;
		} control;
	};

	DMA() = default;

	Channel& getChannel(DMA::Port port) { return m_channels[static_cast<uint32_t>(port)]; }

	uint32_t load32(uint32_t offset) const;
	void store32(uint32_t offset, uint32_t value, DMA::Port& activePort);
private:
	Channel m_channels[7];
	
	union {
		struct {
			uint32_t MDECinPriority      : 3; // 0-2
			uint32_t MDECinMasterEnable  : 1; // 3
			uint32_t MDECoutPriority     : 3; // 4-6
			uint32_t MDECoutMasterEnable : 1; // 7
			uint32_t GPUPriority         : 3; // 8-10
			uint32_t GPUMasterEnable     : 1; // 11
			uint32_t CDROMPriority       : 3; // 12-14
			uint32_t CDROMMasterEnable   : 1; // 15
			uint32_t SPUPriority         : 3; // 16-18
			uint32_t SPUMasterEnable     : 1; // 19
			uint32_t PIOPriority         : 3; // 20-22
			uint32_t PIOMasterEnable     : 1; // 23
			uint32_t OTCPriority         : 3; // 24-26
			uint32_t OTCMasterEnable     : 1; // 27
			uint32_t unknown             : 4; // 28-31
		} fields;
		uint32_t word = 0x07654321;
	} m_control;
	
	union {
		struct {
			uint32_t unknown        : 6; // 0-5
			uint32_t alwaysZero     : 9; // 6-14
			uint32_t forceIRQ       : 1; // 15
			uint32_t channelEnables : 7; // 16-22
			uint32_t masterEnable   : 1; // 23
			uint32_t channelFlags   : 7; // 24-30
			uint32_t masterFlag     : 1; // 31
		} fields;
		uint32_t word = 0;
	} m_interrupt;
};
