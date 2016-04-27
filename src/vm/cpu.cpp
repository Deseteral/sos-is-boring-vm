#include <cstdlib>
#include <cstring>

#include "cpu.hpp"
#include "../conversion.hpp"
#include "../opcodes.hpp"

#define VALIDATE_ARGS(val1, val2)\
VALIDATE_ARG(val1)\
VALIDATE_ARG(val2)

#define VALIDATE_ARG(value)\
{\
	if (((value) & ~0x80) > 0x12)\
		return ERR_INVALID_ARG;\
}

#define VALIDATE_PC()\
{\
	if (bytes2word(this->pc) >= mem_size)\
		return ERR_PC_BOUNDARY;\
}

static inline u32 opcode(u32 instruction)
{
	return instruction >> 24;
}

static inline u32 data(u32 instruction)
{
	return instruction & 0xffffff;
}

static inline u32 byte(u32 instruction, u8 number)
{
	return (instruction >> (8 * (3 - number))) & 0xff;
}

CPU::CPU() :
	lc(&this->registers[0x10 * sizeof(u32)]),
	sp(&this->registers[0x11 * sizeof(u32)]),
	pc(&this->registers[0x12 * sizeof(u32)]),
	mem_size(0), memory(NULL),
	registers({0}), stack({0}), flags({false, false, false, false}),
	extension({0})
{}

CPU::~CPU()
{
	free(this->memory);
}

u32
CPU::CurrentInstruction()
{
	return bytes2word(this->memory + bytes2word(this->pc) * sizeof(u32));
}

u32
CPU::LastInstruction()
{
	return bytes2word(this->memory + (bytes2word(this->pc) - 1) * sizeof(u32));
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

u8*
CPU::WhichRegister(u8 value)
{
	u8 *reg_ptr;
	reg_ptr = &this->registers[(value & ~0x80) * sizeof(u32)];
	if (value & 0x80)
	{
		if (*reg_ptr < this->mem_size)
			return &this->memory[*reg_ptr * sizeof(u32)];
		else
			return NULL;
	}
	return reg_ptr;
}

CPU::ProgramState
CPU::Tick()
{
	VALIDATE_PC()
	u8 *ptr1, *ptr2;
	s64 result;
	u32 instruction = this->CurrentInstruction();
	bytes_add(this->pc, 1);
	switch (opcode(instruction))
	{
		case OP_NOP:
			break;
		case OP_HCF:
			return HALTED;
		case OP_MOV:
			if (byte(instruction, 3))
			{
				VALIDATE_ARG(byte(instruction, 1))
				VALIDATE_PC()
				ptr1 = this->WhichRegister(byte(instruction, 1));
				if (!ptr1)
					return ERR_ADDRESS_BOUNDARY;
				memcpy(
					ptr1,
					this->memory + bytes2word(this->pc) * sizeof(u32),
					sizeof(u32)
				);
				bytes_add(this->pc, 1);
			}
			else
			{
				VALIDATE_ARGS(byte(instruction, 1), byte(instruction, 2))
				ptr1 = this->WhichRegister(byte(instruction, 1));
				ptr2 = this->WhichRegister(byte(instruction, 2));
				if (!ptr1 || !ptr2)
					return ERR_ADDRESS_BOUNDARY;
				word2bytes(bytes2word(ptr2), ptr1);
			}
			break;
		case OP_ADD:
			VALIDATE_ARGS(byte(instruction, 1), byte(instruction, 2))
			ptr1 = this->WhichRegister(byte(instruction, 1));
			ptr2 = this->WhichRegister(byte(instruction, 2));
			if (!ptr1 || !ptr2)
				return ERR_ADDRESS_BOUNDARY;
			result = (s64)(s32)bytes2word(ptr1) + (s32)bytes2word(ptr2);
			word2bytes(result, ptr1);
			this->flags.carry = (bool)(result != (s32)result);
			break;
		// TODO: add remaining opcodes
		case _OP_SIZE:
			if (this->mem_size < data(instruction)) {
				this->extension.required_memory = data(instruction);
				return _ERR_SIZE;
			}
			break;
		case _OP_DEBUG:
			this->extension.debug_info = data(instruction);
			break;
		default:
			this->_SetErroredLine();
			return ERR_INVALID_OPCODE;
	}
	return OK;
}

void
CPU::_SetErroredLine()
{
	if (!this->extension.debug_info)
		return;
	DebugSymbol key;
	for (
		u32 word_index = this->extension.debug_info;
		word_index < this->mem_size;
		word_index += key.next()
	) {
		key.value = bytes2word(this->memory + word_index * sizeof(u32));
		if (word_index == bytes2word(this->pc))
		{
			this->extension.errored_line =
				(char *)(this->memory + word_index * sizeof(u32));
			return;
		}
		else if (key.word() > bytes2word(this->pc)) // instruction not present
		                                            // in the source file
			break;
		else if (!key.next())  // no more lines follows
			break;
	}
	this->extension.errored_line = NULL;
}
