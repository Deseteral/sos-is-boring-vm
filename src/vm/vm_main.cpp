#include <cstdio>

#include "cpu.hpp"

int main(int argc, char *argv[])
{
	printf("This is vm\n");

	CPU cpu;
	cpu.Initialize(128 * 1024);

	return 0;
}
