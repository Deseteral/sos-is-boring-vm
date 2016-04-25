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
	this->extension.debug_info = NULL;
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
	int byte = '\0';
	for (u32 word_index = 0; byte != EOF && word_index < this->mem_size; ++word_index)
		for (int shift = 8 * (sizeof(u32) - 1); shift >= 0 && (byte = getc(input)) != EOF; shift -= 8)
			memory[word_index] |= byte << shift;
	return byte == EOF;
}

CPU::ProgramState
CPU::Tick()
{
	if (pc >= mem_size)
		return ERR_PC_BOUNDARY;
	u32 instruction = this->memory[this->pc++];
	u32 data = instruction & ~(0xff << 24);
	switch (instruction & (0xff << 24))
	{
		case OP_NOP:
			printf("NOP!: %u, %#.8X\n", pc, instruction);
			break;
		// TODO: add remaining opcodes
		case _OP_SIZE:
			if (this->mem_size < data) {
				this->extension.required_memory = data;
				return _ERR_SIZE;
			}
			break;
		case _OP_DEBUG:
			this->extension.debug_info =  this->memory + data;
			break;
		default:
			this->set_errored_line();
			return ERR_INVALID_OPCODE;
	}
	return OK;
}

void
CPU::set_errored_line()
{
	if (!this->extension.debug_info)
		return;
	for (
		u32 *key = this->extension.debug_info;
		key < this->memory + this->mem_size;
		key += *key & 0xffff
	)
		if (this->pc == (*key >> 16))
		{
			this->extension.errored_line = (char *)(key + 1);
			return;
		}
		else if (pc < (*key >> 16))  // instruction not present in source file
			break;
		else if (!(*key & 0xffff))  // no more lines follows
			break;
	this->extension.errored_line = NULL;
}
