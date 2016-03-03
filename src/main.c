#include <stdio.h>

#include "cpu.h"

int main()
{
	CPU cpu;
	cpu.pc = 2;

	printf("%d\n", cpu.pc);

	return 0;
}
