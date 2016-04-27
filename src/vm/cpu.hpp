#pragma once

#include <cstdio>
#include "../types.hpp"

struct CPU
{
	enum ProgramState
	{
		HALTED, OK,
		ERR_ADDRESS_BOUNDARY, ERR_PC_BOUNDARY,
		ERR_INVALID_ARG, ERR_INVALID_OPCODE, ERR_ZERO_DIVISION,
		ERR_STACK_EMPTY, ERR_STACK_OVERFLOW,
		_ERR_SIZE
	};
	struct DebugSymbol {
		u32 value;

		u16 word() { return this->value >> 16;    }
		u16 next() { return this->value & 0xffff; }
	};
	static const int NumberOfRegisters = 16 + 3;
	static const int StackSize = 256;

	u8 *lc, *sp, *pc;
	u32 mem_size;
	u8 *memory;
	u8 registers[NumberOfRegisters * sizeof(u32)];
	u8 stack[StackSize * sizeof(u32)];
	struct {
		bool zero;
		bool greater;
		bool lower;
		bool carry;
	} flags;
	union {
		u32 debug_info;
		char *errored_line;
		int required_memory;
	} extension;

	CPU();
	~CPU();
	u32 CurrentInstruction();
	u32 LastInstruction();
	bool Initialize(u32);
	bool Load(FILE *);
	u8 *WhichRegister(u8);
	ProgramState Tick();

	void _SetErroredLine();
};
