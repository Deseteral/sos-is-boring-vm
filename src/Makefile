CXX= g++
CXXFLAGS= -g -Wall -pedantic
LFLAGS=
OBJS= main.o cpu.o

all: vm

vm: ${OBJS}
	${CXX} -o vm.elf ${CXXFLAGS} ${OBJS} ${LFLAGS}

main.o: main.cpp cpu.h types.h
	${CXX} -c -o main.o ${CXXFLAGS} main.cpp

cpu.o: cpu.cpp cpu.h types.h opcodes.h
	${CXX} -c -o cpu.o ${CXXFLAGS} cpu.cpp

clean:
	rm -f ${OBJS}

cleanall: clean
	rm -f vm.elf
