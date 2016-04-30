#pragma once

#define OP_NOP  0x00
#define OP_HCF  0x01
#define OP_MOV  0x02
#define OP_ADD  0x03
#define OP_SUB  0x04
#define OP_MUL  0x05
#define OP_DIV  0x06
#define OP_MOD  0x07
#define OP_SAL  0x08
#define OP_SAR  0x09
#define OP_INC  0x0A
#define OP_DEC  0x0B
#define OP_SHL  0x0C
#define OP_SHR  0x0D
#define OP_AND  0x0E
#define OP_OR   0x0F
#define OP_XOR  0x10
#define OP_NOT  0x11
#define OP_PUSH 0x12
#define OP_POP  0x13
#define OP_JMP  0x14
#define OP_JMR  0x15
#define OP_CMP  0x16
#define OP_JZ   0x17
#define OP_JNZ  0x18
#define OP_JG   0x19
#define OP_JNG  0x1A
#define OP_JL   0x1B
#define OP_JNL  0x1C
#define OP_JC   0x1D
#define OP_JNC  0x1E
#define OP_LOOP 0x1F

// extensions
#define _OP_SIZE  0xF0
#define _OP_DEBUG 0xF1

// values
#define VAL_R0  0x00
#define VAL_R1  0x01
#define VAL_R2  0x02
#define VAL_R3  0x03
#define VAL_R4  0x04
#define VAL_R5  0x05
#define VAL_R6  0x06
#define VAL_R7  0x07
#define VAL_R8  0x08
#define VAL_R9  0x09
#define VAL_R10 0x0A
#define VAL_R11 0x0B
#define VAL_R12 0x0C
#define VAL_R13 0x0D
#define VAL_R14 0x0E
#define VAL_R15 0x0F

#define VAL_LC  0x10
#define VAL_SP  0x11
#define VAL_PC  0x12

#define VAL_AT_ADDR_R0  0x80
#define VAL_AT_ADDR_R1  0x81
#define VAL_AT_ADDR_R2  0x82
#define VAL_AT_ADDR_R3  0x83
#define VAL_AT_ADDR_R4  0x84
#define VAL_AT_ADDR_R5  0x85
#define VAL_AT_ADDR_R6  0x86
#define VAL_AT_ADDR_R7  0x87
#define VAL_AT_ADDR_R8  0x88
#define VAL_AT_ADDR_R9  0x89
#define VAL_AT_ADDR_R10 0x8A
#define VAL_AT_ADDR_R11 0x8B
#define VAL_AT_ADDR_R12 0x8C
#define VAL_AT_ADDR_R13 0x8D
#define VAL_AT_ADDR_R14 0x8E
#define VAL_AT_ADDR_R15 0x8F

#define VAL_AT_ADDR_LC  0x90
#define VAL_AT_ADDR_SP  0x91
#define VAL_AT_ADDR_PC  0x92
