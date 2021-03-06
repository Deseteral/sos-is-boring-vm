#include <cerrno>
#include <cstdio>
#include <getopt.h>

#include "cpu.hpp"

void print_debug_info(CPU *cpu)
{
	if (cpu->_SetErroredLine())
		fprintf(stderr, "line %s\n", cpu->extension.errored_line);
	fprintf(stderr, "errored instruction: 0x%.8X\n", cpu->LastInstruction());
	return;
}

int main(int argc, char *argv[])
{
	const char *const Help = "SOS is boring VM - simple virtual machine\n"
	"Syntax: vm [arguments] [input file]\n"
	"Arguments:\n"
	"  -d FILE, --dump FILE  print memory dump to a file\n"
	"  -h, --help            print this help text\n"
	"  -s SIZE, --size SIZE  set memory size in 32-bit words (<= 2^24)\n"
	"  -u, --usage           short usage information\n"
	"  -v, --version         display program version";
	const struct option LongOptions[] = {
		{"dump", 1, NULL, 'd'},
		{"help", 0, NULL, 'h'},
		{"size", 1, NULL, 's'},
		{"usage", 0, NULL, 'u'},
		{"version", 0, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};
	u32 mem_size = 16 * 1024;
	char *input_file = NULL;
	char *dump_file = NULL;
	for (int option, long_option_index; (option = getopt_long(argc, argv, "d:hs:uv", LongOptions, &long_option_index)) != -1;)
		switch (option)
		{
			case 'd':
				dump_file = optarg;
				break;
			case 'h':
				puts(Help);
				return 0;
			case 's':
				if (sscanf(optarg, "%u", &mem_size) != 1)
				{
					fprintf(stderr, "Invalid memory size parameter \"%s\".\n", optarg);
					return EINVAL;
				}
				if (mem_size > CPU::MaxMemorySize)
				{
					fprintf(
						stderr,
						"Memory size must not exceed %d.\n",
						CPU::MaxMemorySize
					);
					return EINVAL;
				}
				break;
			case 'u':
				puts("Syntax: vm [-s SIZE|--size SIZE] [INPUT_FILE]\n"
				"           [-h|--help] [-u|--usage] [-v|--version]");
				return 0;
			case 'v':
				puts("vm compiled on " __DATE__ " at " __TIME__ "\n"
				"Copyright (C) 2016 Deseteral <deseteral@gmail.com>\n"
				"Copyright (C) 2016 Subc2 <subc2@wp.pl>");
				return 0;
			default:
				return EINVAL;
		}
	if (optind == argc - 1)  // 1 unknown parameter (input file name)
		input_file = argv[optind];
	else if (optind != argc)  // more unknown parameters
	{
		fputs("Invalid argument(s) found.\n", stderr);
		return EINVAL;
	}
	FILE *input = (input_file == NULL ? stdin : fopen(input_file, "rb"));
	if (input == NULL)
	{
		fprintf(stderr, "Cannot open file \"%s\" for reading.\n", input_file);
		return ENOENT;
	}
	CPU cpu;
	if (!cpu.Initialize(mem_size))
	{
		fputs("Cannot allocate memory.\n", stderr);
		return ENOMEM;
	}
	if (!cpu.Load(input)) {
		fputs("Memory size is too small to store the program.\n", stderr);
		return 1;
	}
	if (input_file)
		fclose(input);
	CPU::ProgramState state;
	while ((state = cpu.Tick()) == CPU::OK)
		continue;
	if (dump_file != NULL)
	{
		FILE *dump = fopen(dump_file, "wb");
		if (dump == NULL)
		{
			fprintf(stderr, "Cannot open file \"%s\" for writing.\n", dump_file);
			return ENOENT;
		}
		for (u32 i = 0; i < cpu.mem_size * sizeof(u32); ++i)
			putc(cpu.memory[i], dump);
		fclose(dump);
	}
	switch (state)
	{
		case CPU::HALTED:
			fputs("Program exited normally.\n", stderr);
			break;
		case CPU::ERR_ADDRESS_BOUNDARY:
			fputs("Address boundary error.\n", stderr);
			print_debug_info(&cpu);
			return 1;
		case CPU::ERR_PC_BOUNDARY:
			fputs("PC boundary error.\n", stderr);
			return 1;
		case CPU::ERR_INVALID_ARG:
			fputs("Invalid operation argument error.\n", stderr);
			print_debug_info(&cpu);
			return 1;
		case CPU::ERR_INVALID_OPCODE:
			fputs("Invalid operation code error.\n", stderr);
			print_debug_info(&cpu);
			return 1;
		case CPU::ERR_ZERO_DIVISION:
			fputs("Zero division error.\n", stderr);
			print_debug_info(&cpu);
			return 1;
		case CPU::ERR_STACK_EMPTY:
			fputs("POP operation on empty stack.\n", stderr);
			print_debug_info(&cpu);
			return 1;
		case CPU::ERR_STACK_OVERFLOW:
			fputs("PUSH operation on full stack.\n", stderr);
			print_debug_info(&cpu);
			return 1;
		case CPU::_ERR_SIZE:
			fprintf(
				stderr,
				"Program explicitly said it needs memory of size %d to run.\n",
				cpu.extension.required_memory
			);
			return 1;
		default:
			fputs("Unknown error occurred.\n", stderr);
			print_debug_info(&cpu);
			return 1;
	}
	return 0;
}
