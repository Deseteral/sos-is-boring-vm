#include "cpu.hpp"

#include <cstdlib>

void
CPU::Initialize(u32 mem_size)
{
	this->pc = 0;
	this->memory = (u8*) malloc(mem_size);
}

void
CPU::Tick()
{
	u32 opcode = this->memory[this->pc++];

	// TODO: Implement instruction set based on documentation
	// switch (opcode) {
	// 	case 0x0:
	// 	break;
	// }
}
