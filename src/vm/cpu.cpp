#include <cstdlib>

#include "cpu.hpp"
#include "../opcodes.hpp"

CPU::CPU()
{
	this->pc = 0;
	this->mem_size = 0;
	this->memory = NULL;
	this->flags.zero = false;
	this->flags.greater = false;
	this->flags.lower = false;
	this->flags.carry = false;
}

CPU::~CPU()
{
	free(this->memory);
}

bool
CPU::Initialize(u32 mem_size)
{
	if ((this->memory = (u32 *)calloc(mem_size, sizeof(u32))))
		this->mem_size = mem_size;
	return (bool)this->memory;
}

bool
CPU::Load(FILE *input)
{
	int byte;
	u32 byte_num = 0;
	while ((byte = getc(input)) != EOF && byte_num < this->mem_size * sizeof(u32))
		this->memory[byte_num++] = byte;
	return byte == EOF;
}

CPU::ProgramState
CPU::Tick()
{
	u32 instruction = this->memory[this->pc++];
	switch (instruction & (0xff << 3))
	{
		case OP_NOP:
			break;
		// TODO: add remaining opcodes
		default:
			return ERR_INVALID_OPCODE;
	}
	return HALTED;
}
