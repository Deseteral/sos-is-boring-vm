#include <cstdlib>
#include "cpu.hpp"

bool
CPU::Initialize(u32 mem_size)
{
	this->mem_size = mem_size;
	this->memory = (u32 *)calloc(mem_size, sizeof(u32));
	return (bool)this->memory;
}

bool
CPU::Load(FILE *input)
{
	int byte;
	u32 byte_num = 0;
	while ((byte = getc(input)) != EOF && byte_num < this->mem_size * sizeof(u32))
		this->memory[byte_num++] = byte;
	this->pc = 0;
	return byte == EOF && byte_num <= this->mem_size * sizeof(u32);
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
