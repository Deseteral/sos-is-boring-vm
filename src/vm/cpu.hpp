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

	u32 pc;
	u32 mem_size;
	u32 *memory;
	struct {
		bool zero;
		bool greater;
		bool lower;
		bool carry;
	} flags;
	union {
		u32 *debug_info;
		char *errored_line;
		int required_memory;
	} extension;

	CPU();
	~CPU();
	bool Initialize(u32);
	bool Load(FILE *);
	ProgramState Tick();

	void set_errored_line();
};
