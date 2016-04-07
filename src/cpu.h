#pragma once

#include "types.h"

struct CPU
{
	u32 pc;
	u8* memory;

	void Initialize(u32 mem_size);
  void Tick();
};
