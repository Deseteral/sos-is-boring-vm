#pragma once

#include "types.h"

typedef struct CPU
{
	u32 pc;
	u8* memory;
} CPU;

void CPU_Initialize(CPU* cpu, u32 mem_size);
void CPU_Tick(CPU* cpu);
