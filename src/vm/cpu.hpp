#pragma once

#include "../types.hpp"

struct CPU
{
	u32 pc;
	u8* memory;

	void Initialize(u32 mem_size);
	void Tick();
};
