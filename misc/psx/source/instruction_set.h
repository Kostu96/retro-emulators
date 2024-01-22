#pragma once
#include "type_helpers.h"

class CPU;

class InstructionSet
{
public:
	using InstFuncType = void (*)(CPU&, Instruction);
	static InstFuncType Fns[];
private:
	static void invalid(CPU& cpu, Instruction instruction);
	static void zero(CPU& cpu, Instruction instruction);
	static void bxxxxx(CPU& cpu, Instruction instruction);
	static void SLL(CPU& cpu, Instruction instruction);
	static void SRL(CPU& cpu, Instruction instruction);
	static void SRA(CPU& cpu, Instruction instruction);
	static void SLLV(CPU& cpu, Instruction instruction);
	static void SRLV(CPU& cpu, Instruction instruction);
	static void SRAV(CPU& cpu, Instruction instruction);
	static void JR(CPU& cpu, Instruction instruction);
	static void JALR(CPU& cpu, Instruction instruction);
	static void SYSCALL(CPU& cpu, Instruction instruction);
	static void BREAK(CPU& cpu, Instruction instruction);
	static void MFHI(CPU& cpu, Instruction instruction);
	static void MTHI(CPU& cpu, Instruction instruction);
	static void MFLO(CPU& cpu, Instruction instruction);
	static void MTLO(CPU& cpu, Instruction instruction);
	static void DIV(CPU& cpu, Instruction instruction);
	static void DIVU(CPU& cpu, Instruction instruction);
	static void MULT(CPU& cpu, Instruction instruction);
	static void MULTU(CPU& cpu, Instruction instruction);
	static void ADD(CPU& cpu, Instruction instruction);
	static void ADDU(CPU& cpu, Instruction instruction);
	static void SUB(CPU& cpu, Instruction instruction);
	static void SUBU(CPU& cpu, Instruction instruction);
	static void AND(CPU& cpu, Instruction instruction);
	static void OR(CPU& cpu, Instruction instruction);
	static void XOR(CPU& cpu, Instruction instruction);
	static void NOR(CPU& cpu, Instruction instruction);
	static void SLT(CPU& cpu, Instruction instruction);
	static void SLTU(CPU& cpu, Instruction instruction);
	static void J(CPU& cpu, Instruction instruction);
	static void JAL(CPU& cpu, Instruction instruction);
	static void BEQ(CPU& cpu, Instruction instruction);
	static void BNE(CPU& cpu, Instruction instruction);
	static void BLEZ(CPU& cpu, Instruction instruction);
	static void BGTZ(CPU& cpu, Instruction instruction);
	static void ADDI(CPU& cpu, Instruction instruction);
	static void ADDIU(CPU& cpu, Instruction instruction);
	static void SLTI(CPU& cpu, Instruction instruction);
	static void SLTIU(CPU& cpu, Instruction instruction);
	static void ANDI(CPU& cpu, Instruction instruction);
	static void ORI(CPU& cpu, Instruction instruction);
	static void XORI(CPU& cpu, Instruction instruction);
	static void LUI(CPU& cpu, Instruction instruction);
	static void cop0(CPU& cpu, Instruction instruction);
	static void cop1(CPU& cpu, Instruction instruction);
	static void cop2(CPU& cpu, Instruction instruction);
	static void cop3(CPU& cpu, Instruction instruction);
	static void MFC0(CPU& cpu, Instruction instruction);
	static void MTC0(CPU& cpu, Instruction instruction);
	static void RFE(CPU& cpu, Instruction instruction);
	static void LB(CPU& cpu, Instruction instruction);
	static void LH(CPU& cpu, Instruction instruction);
	static void LWL(CPU& cpu, Instruction instruction);
	static void LW(CPU& cpu, Instruction instruction);
	static void LBU(CPU& cpu, Instruction instruction);
	static void LHU(CPU& cpu, Instruction instruction);
	static void LWR(CPU& cpu, Instruction instruction);
	static void SB(CPU& cpu, Instruction instruction);
	static void SH(CPU& cpu, Instruction instruction);
	static void SWL(CPU& cpu, Instruction instruction);
	static void SW(CPU& cpu, Instruction instruction);
	static void SWR(CPU& cpu, Instruction instruction);
	static void LWC0(CPU& cpu, Instruction instruction);
	static void	LWC1(CPU& cpu, Instruction instruction);
	static void	LWC2(CPU& cpu, Instruction instruction);
	static void	LWC3(CPU& cpu, Instruction instruction);
	static void	SWC0(CPU& cpu, Instruction instruction);
	static void	SWC1(CPU& cpu, Instruction instruction);
	static void	SWC2(CPU& cpu, Instruction instruction);
	static void	SWC3(CPU& cpu, Instruction instruction);
};
