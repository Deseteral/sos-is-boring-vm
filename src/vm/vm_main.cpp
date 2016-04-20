#include <cerrno>
#include <cstdio>
#include <getopt.h>

#include "cpu.hpp"

int main(int argc, char *argv[])
{
	const char *const Help = "SOS is boring VM - simple virtual machine\n"
	"Syntax: vm [arguments] [input file]\n"
	"Arguments:\n"
	"  -h, --help            print this help text\n"
	"  -s SIZE, --size SIZE  set memory size in 32-bit words\n"
	"  -u, --usage           short usage information\n"
	"  -v, --version         display program version";
	const struct option LongOptions[] = {
		{"help", 0, NULL, 'h'},
		{"usage", 0, NULL, 'u'},
		{"version", 0, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};
	u32 mem_size = 16 * 1024;
	char *input_file = NULL;
	for (int option, long_option_index; (option = getopt_long(argc, argv, "hs:uv", LongOptions, &long_option_index)) != -1;)
		switch (option)
		{
			case 'h':
				puts(Help);
				return 0;
			case 's':
				if (sscanf(optarg, "%u", &mem_size) != 1) {
					fprintf(stderr, "Invalid memory size parameter \"%s\".\n", optarg);
					return EINVAL;
				}
				break;
			case 'u':
				puts("Syntax: vm [-s SIZE|--size SIZE] [INPUT_FILE]\n"
				"           [-h|--help] [-u|--usage] [-v|--version]");
				return 0;
			case 'v':
				puts("vm compiled on " __DATE__ " at " __TIME__ "\n"
				"Copyright (C) 2016 Deseteral <deseteral@gmail.com>");
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
		fputs("Memory size is to small to store the program.\n", stderr);
		return 1;
	}
	if (input_file)
		fclose(input);
	ProgramState state;
	while ((state = cpu.Tick()) == OK)
		continue;
	switch (state)
	{
		case HALTED:
			fputs("Program exited normally.\n", stderr);
			break;
		case ERR_ADDRESS_BOUNDARY:
			fputs("Address boundary error.\n", stderr);
			return 1;
		case ERR_INVALID_OPCODE:
			fputs("Invalid operation error.\n", stderr);
			return 1;
		case ERR_ZERO_DIVISION:
			fputs("Zero division error.\n", stderr);
			return 1;
		case ERR_STACK_EMPTY:
			fputs("POP operation on empty stack.\n", stderr);
			return 1;
		case ERR_STACK_OVERFLOW:
			fputs("PUSH operation on full stack.\n", stderr);
			return 1;
	}
	return 0;
}
