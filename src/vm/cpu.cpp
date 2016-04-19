#include "cpu.hpp"

#include <cstdlib>

bool
CPU::Initialize(u32 mem_size)
{
	this->pc = 0;
	this->memory = (u32 *)calloc(mem_size, sizeof(u32));
	return this->memory ? true : false;
}

ProgramState
CPU::Tick()
{
	u32 opcode = this->memory[this->pc++];

	// TODO: Implement instruction set based on documentation
	// switch (opcode) {
	// 	case 0x0:
	// 	break;
	// }
	return HALTED;
}
