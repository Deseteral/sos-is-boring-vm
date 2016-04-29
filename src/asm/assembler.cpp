#include "assembler.hpp"

#include <cstring>
#include <vector>

#include "../opcodes.hpp"

void StringToUpperCase(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		  str[i] -= 32;
	}
}

#define TOKEN_TYPE_NOTHING  0
#define TOKEN_TYPE_REGISTER 1
#define TOKEN_TYPE_VALUE    2

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
};

void Assemble(FILE *input_file, FILE *output_file)
{
	char opcode[32];
	u32 pc = 0;
	std::vector<Instruction> instructions;

	while (fscanf(input_file, "%s", opcode) != EOF)
	{
		StringToUpperCase(opcode);

		bool is_valid = true;

		Instruction ins;
		if (strcmp(opcode, "NOP") == 0)
			ins.opcode = OP_NOP;
		else if (strcmp(opcode, "HCF") == 0)
			ins.opcode = OP_HCF;
		else
			is_valid = false;

		if (is_valid)
		{
			ins.address = pc;
			pc += 4;
			
			instructions.push_back(ins);
		}
	}

	// Write to file
	for (int i = 0; i < instructions.size(); i++)
	{
		fwrite(&instructions[i].opcode, 1, 1, output_file);

		fwrite(&instructions[i].values[0].value, 1, 1, output_file);
		fwrite(&instructions[i].values[1].value, 1, 1, output_file);
		fwrite(&instructions[i].values[2].value, 1, 1, output_file);
	}
}
