#pragma once
#include "bios.h"
#include "ram.h"
#include "dma.h"
#include "gpu.h"
#include "timers.h"
#include "cd_rom.h"

class MemoryMap
{
public:
	MemoryMap(Renderer& renderer) : m_gpu(renderer) {}

	uint8_t load8(uint32_t address) const;
	uint16_t load16(uint32_t address) const;
	uint32_t load32(uint32_t address) const;
	void store8(uint32_t address, uint8_t value);
	void store16(uint32_t address, uint16_t value);
	void store32(uint32_t address, uint32_t value);
private:
	static uint32_t maskRegion(uint32_t address);
	void DMATransfer(DMA::Port port);
	void DMATransferBlock(DMA::Port port);
	void DMATransferLinkedList(DMA::Port port);

	static const uint32_t REGION_MASK[];
	static const AddressRange RAM_RANGE;
	static const AddressRange EXPANSION1_RANGE;
	static const AddressRange MEMORY_CONTROL_RANGE;
	static const AddressRange RAM_SIZE_RANGE;
	static const AddressRange IRQ_CONTROL_RANGE;
	static const AddressRange DMA_RANGE;
	static const AddressRange TIMERS_RANGE;
	static const AddressRange CDROM_RANGE;
	static const AddressRange GPU_RANGE;
	static const AddressRange SPU_RANGE;
	static const AddressRange EXPANSION2_RANGE;
	static const AddressRange BIOS_RANGE;
	static const AddressRange CACHE_CONTROL_RANGE;
	
	BIOS m_bios;
	RAM m_ram;
	DMA m_dma;
	GPU m_gpu;
	Timers m_timers;
	CDROM m_cdROM;
};
