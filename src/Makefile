CXX=g++
CXXFLAGS=-g -Wall -Wpedantic
LFLAGS=
OBJS=main.o cpu.o

all: vm

vm: ${OBJS}
	${CXX} -o vm ${CXXFLAGS} ${OBJS} ${LFLAGS}

main.o: main.cpp cpu.h types.h
	${CXX} -c -o main.o ${CXXFLAGS} main.cpp

cpu.o: cpu.cpp cpu.h types.h
	${CXX} -c -o cpu.o ${CXXFLAGS} cpu.cpp

clean:
	rm -f ${OBJS}

cleanall: clean
	rm -f vm
