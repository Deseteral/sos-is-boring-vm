#include <cstdlib>

#include "cpu.hpp"
#include "../opcodes.hpp"

CPU::CPU() :
	pc(0), lc(0), sp(0), mem_size(0), memory(NULL),
	registers({0}), stack({0}), flags({false, false, false, false}),
	extension({0})
{}

CPU::~CPU()
{
	free(this->memory);
}

u32
CPU::Instruction(u32 counter)
{
	return
		(this->memory[counter * sizeof(u32) + 0] << 24) |
		(this->memory[counter * sizeof(u32) + 1] << 16) |
		(this->memory[counter * sizeof(u32) + 2] << 8) 	|
		(this->memory[counter * sizeof(u32) + 3] << 0);
}

bool
CPU::Initialize(u32 mem_size)
{
	if ((this->memory = (u8 *)malloc(mem_size * sizeof(u32))))
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
	if (pc >= mem_size)
		return ERR_PC_BOUNDARY;
	u32 instruction = this->Instruction(this->pc++);
	u32 data = instruction & ~(0xff << 24);
	switch (instruction >> 24)
	{
		case OP_NOP:
			break;
		// TODO: add remaining opcodes
		case _OP_SIZE:
			if (this->mem_size < data) {
				this->extension.required_memory = data;
				return _ERR_SIZE;
			}
			break;
		case _OP_DEBUG:
			this->extension.debug_info = data;
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
	DebugSymbol key;
	for (
		u32 word_index = this->extension.debug_info;
		word_index < this->mem_size;
		word_index += key.next()
	) {
		key.value = this->Instruction(word_index);
		if (this->pc == word_index)
		{
			this->extension.errored_line =
				(char *)(this->memory + word_index * sizeof(u32));
			return;
		}
		else if (pc < key.word())  // instruction not present in source file
			break;
		else if (!key.next())  // no more lines follows
			break;
	}
	this->extension.errored_line = NULL;
}
