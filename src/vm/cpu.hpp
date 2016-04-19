#pragma once

#include "../types.hpp"

enum ProgramState
{
	HALTED, OK,
	ERR_ADDRESS_BOUNDARY, ERR_INVALID_OPCODE, ERR_ZERO_DIVISION,
	ERR_STACK_EMPTY, ERR_STACK_OVERFLOW
};

struct CPU
{
	u32 pc;
	u32 *memory;

	bool Initialize(u32 mem_size);
	ProgramState Tick();
};
