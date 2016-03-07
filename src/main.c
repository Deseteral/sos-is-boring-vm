#include <stdio.h>

#include "cpu.h"

int main(int argc, char *argv[])
{
	CPU cpu;
	cpu.pc = 2;

	printf("%d\n", cpu.pc);

	return 0;
}
