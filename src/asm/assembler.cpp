#include "assembler.hpp"

#include <cstdlib>
#include <cstring>
#include <vector>

#include "../utils.hpp"
#include "../opcodes.hpp"

#define IF_OPCODE(VALUE)\
	if (strcmp(opcode, VALUE) == 0)

#define READ_VALUE_A()\
	char value_a[32];\
	fscanf(input_file, "%s", value_a);\
	cstring_to_upper_case(value_a);

#define READ_VALUE_AB()\
	char value_a[32];\
	char value_b[32];\
	fscanf(input_file, "%s %s", value_a, value_b);\
	cstring_to_upper_case(value_a);\
	cstring_to_upper_case(value_b);

#define TOKEN_TYPE_NOTHING  0
#define TOKEN_TYPE_REGISTER 1
#define TOKEN_TYPE_NUMBER   2
#define TOKEN_TYPE_FLAG     3

struct Token
{
	u32 token_type = TOKEN_TYPE_NOTHING;
	u8 value = 0;
};

struct Instruction
{
	u32 address;
	u8 opcode;
	Token values[3];

	bool uses_next_word = false;
	u32 next_word = 0;
};

bool which_register(char *value, u8 &reg)
{
	if (strlen(value) < 2)
		return false;

	if (value[0] == 'R')
		reg = (VAL_R0 + atoi(&value[1]));
	else if (streq(value, "LC"))
		reg = 0x10;
	else if (streq(value, "SP"))
		reg = 0x11;
	else if (streq(value, "PC"))
		reg = 0x12;
	else
		return false;

	return true;
}

void assemble(FILE *input_file, FILE *output_file)
{
	char opcode[32];
	u32 pc = 0;
	std::vector<Instruction> instructions;

	while (fscanf(input_file, "%s", opcode) != EOF)
	{
		cstring_to_upper_case(opcode);

		Instruction ins;
		bool is_valid = true;

		IF_OPCODE("NOP")
		{
			ins.opcode = OP_NOP;
		}
		else IF_OPCODE("HCF")
		{
			ins.opcode = OP_HCF;
		}
		else IF_OPCODE("MOV")
		{
			ins.opcode = OP_MOV;

			READ_VALUE_AB()

			// Value C is a flag
			ins.values[2].token_type = TOKEN_TYPE_FLAG;

			// Value A is register
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			which_register(value_a, ins.values[0].value);

			// Value B can be either a register or constant value
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;
			ins.values[2].value = 0;

			bool is_register = which_register(value_b, ins.values[1].value);
			if (!is_register)
			{
				ins.values[1].token_type = TOKEN_TYPE_NUMBER;
				ins.values[2].value = 1;

				ins.uses_next_word = true;

				s32 snw;
				sscanf(value_b, "%i", &snw);
				ins.next_word = (u32)snw;
			}
		}
		else IF_OPCODE("ADD")
		{
			ins.opcode = OP_ADD;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_AB()

			which_register(value_a, ins.values[0].value);
			which_register(value_b, ins.values[1].value);
		}
		else IF_OPCODE("SUB")
		{
			ins.opcode = OP_SUB;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_AB()

			which_register(value_a, ins.values[0].value);
			which_register(value_b, ins.values[1].value);
		}
		else IF_OPCODE("MUL")
		{
			ins.opcode = OP_MUL;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_AB()

			which_register(value_a, ins.values[0].value);
			which_register(value_b, ins.values[1].value);
		}
		else IF_OPCODE("IMUL")
		{
			ins.opcode = OP_IMUL;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_AB()

			which_register(value_a, ins.values[0].value);
			which_register(value_b, ins.values[1].value);
		}
		else IF_OPCODE("DIV")
		{
			ins.opcode = OP_DIV;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_AB()

			which_register(value_a, ins.values[0].value);
			which_register(value_b, ins.values[1].value);
		}
		else IF_OPCODE("IDIV")
		{
			ins.opcode = OP_IDIV;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_AB()

			which_register(value_a, ins.values[0].value);
			which_register(value_b, ins.values[1].value);
		}
		else IF_OPCODE("MOD")
		{
			ins.opcode = OP_MOD;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_AB()

			which_register(value_a, ins.values[0].value);
			which_register(value_b, ins.values[1].value);
		}
		else IF_OPCODE("IMOD")
		{
			ins.opcode = OP_IMOD;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;
			ins.values[1].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_AB()

			which_register(value_a, ins.values[0].value);
			which_register(value_b, ins.values[1].value);
		}
		else IF_OPCODE("INC")
		{
			ins.opcode = OP_INC;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_A()

			which_register(value_a, ins.values[0].value);
		}
		else IF_OPCODE("DEC")
		{
			ins.opcode = OP_DEC;
			ins.values[0].token_type = TOKEN_TYPE_REGISTER;

			READ_VALUE_A()

			which_register(value_a, ins.values[0].value);
		}
		else
			is_valid = false;

		if (is_valid)
		{
			ins.address = pc;
			pc += 4;

			if (ins.uses_next_word)
				pc += 4;

			instructions.push_back(ins);
		}
	}

	// Write to file
	for (u32 i = 0; i < instructions.size(); i++)
	{
		Instruction *ins = &instructions[i];
		fwrite(&ins->opcode, 1, 1, output_file);

		fwrite(&ins->values[0].value, sizeof(u8), 1, output_file);
		fwrite(&ins->values[1].value, sizeof(u8), 1, output_file);
		fwrite(&ins->values[2].value, sizeof(u8), 1, output_file);

		if (ins->uses_next_word)
		{
			u8 bytes[4];
			word2bytes(ins->next_word, bytes);

			fwrite(&bytes, sizeof(u8), 4, output_file);
		}
	}
}
