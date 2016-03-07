#include "cpu.h"

#include <stdlib.h>

void CPU_Initialize(CPU* cpu, u32 mem_size)
{
	cpu->pc = 0;
	cpu->memory = (u8*) malloc(mem_size);
}

void CPU_Tick(CPU* cpu)
{
	u32 opcode = cpu->memory[cpu->pc++];

	// TODO: Implement instruction set based on documentation
	// switch (opcode) {
	// 	case 0x0:
	// 	break;
	// }
}
