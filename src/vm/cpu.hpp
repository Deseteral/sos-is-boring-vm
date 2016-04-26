#pragma once

#include <cstdio>
#include "../types.hpp"

struct CPU
{
	enum ProgramState
	{
		HALTED, OK,
		ERR_ADDRESS_BOUNDARY, ERR_PC_BOUNDARY,
		ERR_INVALID_OPCODE, ERR_ZERO_DIVISION,
		ERR_STACK_EMPTY, ERR_STACK_OVERFLOW,
		_ERR_SIZE
	};
	struct DebugSymbol {
		u32 value;

		u16 word() { return this->value >> 16;    }
		u16 next() { return this->value & 0xffff; }
	};

	u32 pc;
	u32 mem_size;
	u8 *memory;
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
	u32 Instruction(u32);
	bool Initialize(u32);
	bool Load(FILE *);
	ProgramState Tick();

	void set_errored_line();
};
