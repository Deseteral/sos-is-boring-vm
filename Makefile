CXX= c++
CXXFLAGS= -g -Wall -pedantic -std=c++0x
LFLAGS=
OBJS= obj/asm.o obj/assembler.o obj/cpu.o obj/vm.o

all: asm vm
asm: bin/asm.elf
vm: bin/vm.elf

bin/asm.elf: obj/asm.o obj/assembler.o
	mkdir -p bin
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LFLAGS)

bin/vm.elf: obj/vm.o obj/cpu.o
	mkdir -p bin
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LFLAGS)

obj/asm.o: src/asm/asm_main.cpp src/asm/assembler.hpp src/types.hpp
	mkdir -p obj
	$(CXX) -c -o $@ $(CXXFLAGS) $<

obj/vm.o: src/vm/vm_main.cpp src/vm/cpu.hpp src/types.hpp
	mkdir -p obj
	$(CXX) -c -o $@ $(CXXFLAGS) $<

obj/cpu.o: src/vm/cpu.cpp src/vm/cpu.hpp src/opcodes.hpp src/types.hpp \
		src/utils.hpp
	mkdir -p obj
	$(CXX) -c -o $@ $(CXXFLAGS) $<

obj/assembler.o: src/asm/assembler.cpp src/asm/assembler.hpp src/opcodes.hpp \
		src/types.hpp src/utils.hpp
	mkdir -p obj
	$(CXX) -c -o $@ $(CXXFLAGS) $<

clean:
	rm -f $(OBJS)

cleanall: clean
	rm -f bin/asm.elf bin/vm.elf
