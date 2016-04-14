CXX= g++
CXXFLAGS= -g -Wall -pedantic
LFLAGS=
OBJS= obj/asm.o obj/cpu.o obj/vm.o

all: asm vm

asm: obj/asm.o
	mkdir -p bin
	${CXX} -o bin/asm.elf ${CXXFLAGS} obj/asm.o ${LFLAGS}

vm: obj/cpu.o obj/vm.o
	mkdir -p bin
	${CXX} -o bin/vm.elf ${CXXFLAGS} obj/cpu.o obj/vm.o ${LFLAGS}

obj/asm.o: src/asm/asm_main.cpp src/opcodes.hpp src/types.hpp
	mkdir -p obj
	${CXX} -c -o obj/asm.o ${CXXFLAGS} src/asm/asm_main.cpp

obj/vm.o: src/vm/vm_main.cpp src/opcodes.hpp src/types.hpp
	mkdir -p obj
	${CXX} -c -o obj/vm.o ${CXXFLAGS} src/vm/vm_main.cpp

obj/cpu.o: src/vm/cpu.cpp src/opcodes.hpp src/vm/cpu.hpp src/types.hpp
	mkdir -p obj
	${CXX} -c -o obj/cpu.o ${CXXFLAGS} src/vm/cpu.cpp

clean:
	rm -f ${OBJS}

cleanall: clean
	rm -f bin/asm.elf bin/vm.elf
