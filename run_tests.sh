#!/bin/bash

ASM='./bin/asm.elf'
VM='./bin/vm.elf'
TESTSDIR='tests'
OUTDIR='out'

if [ ! -d "$TESTSDIR" ]; then
	echo 'No tests were found.'
	exit 1
fi

if [ ! -x "$ASM" ]; then
	echo "Cannot run $ASM."
	exit 1
elif [ ! -x "$VM" ]; then
	echo "Cannot run $VM."
	exit 1
fi

if [ ! -d "$OUTDIR" ]; then
	mkdir "$OUTDIR"
fi

for I in "$TESTSDIR"/*.asm; do
	I="$(basename $I .asm)"

	eval "$ASM -o $OUTDIR/$I.sos $TESTSDIR/$I.asm"
	cmp "$OUTDIR/$I.sos" "$TESTSDIR/$I.sos" &> /dev/null
	if [ "$?" -ne 0 ]; then
		echo -ne "DIFFERS: \e[31m$I.sos"
	else
		echo -ne "OK: \e[32m$I.sos"
	fi
	echo -e "\e[0m"

	eval "$VM -d $OUTDIR/$I.hex $TESTSDIR/$I.sos &> $OUTDIR/$I.out"
	cmp "$OUTDIR/$I.out" "$TESTSDIR/$I.out" &> /dev/null
	if [ "$?" -ne 0 ]; then
		echo -ne "DIFFERS: \e[31m$I.out"
	else
		echo -ne "OK: \e[32m$I.out"
	fi
	echo -e "\e[0m"

	cmp "$OUTDIR/$I.hex" "$TESTSDIR/$I.hex" &> /dev/null
	if [ "$?" -ne 0 ]; then
		echo -ne "DIFFERS: \e[31m$I.hex"
	else
		echo -ne "OK: \e[32m$I.hex"
	fi
	echo -e "\e[0m"
done

exit
