#include <cerrno>
#include <cstdio>
#include <getopt.h>

#include "assembler.hpp"

int main(int argc, char *argv[])
{
	const char *const Help = "SOS is boring ASM - assembler for SOS VM\n"
	"Syntax: asm [arguments] [input file]\n"
	"Arguments:\n"
	"  -h, --help            print this help text\n"
	"  -u, --usage           short usage information\n"
	"  -v, --version         display program version";

	const struct option LongOptions[] = {
		{"help", 0, NULL, 'h'},
		{"version", 0, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};

	for (int option, long_option_index; (option = getopt_long(argc, argv, "hs:uv", LongOptions, &long_option_index)) != -1;)
	{
		switch (option)
		{
			case 'h':
				puts(Help);
				return 0;
			case 'u':
				puts("Syntax: asm [INPUT_FILE]\n"
				"           [-h|--help] [-u|--usage] [-v|--version]");
				return 0;
			case 'v':
				puts("asm compiled on " __DATE__ " at " __TIME__ "\n"
				"Copyright (C) 2016 Deseteral <deseteral@gmail.com>\n"
				"Copyright (C) 2016 Subc2 <subc2@wp.pl>");
				return 0;
			default:
				return EINVAL;
		}
	}

	char *input_file_name = NULL;
	char *output_file_name = "a.sos";

	if (optind == argc - 1)  // 1 unknown parameter (input file name)
		input_file_name = argv[optind];
	else if (optind != argc)  // more unknown parameters
	{
		fputs("Invalid argument(s) found.\n", stderr);
		return EINVAL;
	}

	FILE *input = (input_file_name == NULL ? stdin : fopen(input_file_name, "r"));
	if (input == NULL)
	{
		fprintf(stderr, "Cannot open file \"%s\" for reading.\n", input_file_name);
		return ENOENT;
	}

//	if (input_file_name != NULL)
//	{
//		char name[256];
//		sscanf(input_file_name, "%s.asm", name);
//		sprintf(output_file_name, "%s.sos", name);
//	}
//	else
//		output_file_name = "a.sos";

	FILE *output = fopen(output_file_name, "wb");

	Assemble(input, output);

	fclose(input);
	fclose(output);

	return 0;
}
