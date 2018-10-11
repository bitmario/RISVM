from enum import IntEnum

labels = {}
label_instances = {}

REGISTERS = {
    "r0": 0,
    "r1": 1,
    "r2": 2,
    "r3": 3,
    "r4": 4,
    "r5": 5,
    "t0": 6,
    "t1": 7,
    "t2": 8,
    "t3": 9,
    "t4": 10,
    "t5": 11,
    "t6": 12,
    "t7": 13,
    "t8": 14,
    "t9": 15,
    "ip": 16,
    "bp": 17,
    "sp": 18,
    "ra": 19
}

class Opcodes(IntEnum):
    NOP = 0x00 # do nothing
    # constants:
    LCONS = 0x10   # store a value in a register e.g.: lcons r0 0xA2 0x00 0x00 0x00
    LCONSW = 0x11  # store a word value in a register e.g.: lconsw r0 0xA2 0x00
    LCONSB = 0x12  # store a byte value in a register e.g.: lconsb r0 0xA2
    # register operations:
    MOV = 0x30     # copy a value between registers e.g.: mov r0 r2
    # stack:
    PUSH = 0x40        # push a register onto the stack e.g.: push r0
    POP = 0x41         # pop the first element of the stack to a register e.g.: pop r0
    POP2 = 0x42        # pop the two first elements of the stack to registers e.g.: pop r0 r1
    DUP = 0x43         # duplicate the last value in the stack e.g.: dup
    # functions
    CALL = 0x50     # set register RA to the next instruction and jump to subroutine, e.g.: call 0x10 0x00
    RET = 0x51      # return to the address of last callee (RA), e.g.: ret
    # memory:
    STOR = 0x60        # copy a value from a register to a heap address e.g.: stor 0x08 0x00 r0
    STORW = 0x61       # copy a word value from a register to a heap address e.g.: storw 0x08 0x00 r0
    STORB = 0x62       # copy a byte value from a register to a heap address e.g.: storb 0x08 0x00 r0
    LOAD = 0x63        # copy a value from a heap address to a register e.g.: load r0 0x08 0x00
    LOADW = 0x64       # copy a word value from a heap address to a register e.g.: loadw r0 0x08 0x00
    LOADB = 0x65       # copy a byte value from a heap address to a register e.g.: loadb r0 0x08 0x00
    MEMCPY = 0x66      # copy N bytes from one memory address S to another address D e.g.: memcpy 0xDD 0xDD 0xSS 0xSS 0xNN 0xNN
    # arithmetic:
    INC = 0x80         # increment the specified register e.g.: inc r0
    INCF = 0x81        # increment a float in the specified register e.g.: incf r0
    DEC = 0x82         # decrement the specified register e.g.: dec r0
    DECF = 0x83        # decrement a float in the specified register e.g.: decf r0
    ADD = 0x84         # sum and store in first reg e.g.: add r0 r2
    ADDF = 0x85        # sum two floats and store in first reg e.g.: addf r0 r2
    SUB = 0x86         # subtract and store in first reg e.g.: sub r0 r2
    SUBF = 0x87        # subtract two floats and store in first reg e.g.: subf r0 r2
    MUL = 0x88         # multiply and store in first reg e.g.: mul r0 r2
    IMUL = 0x89        # multiply and store in first reg e.g.: mul r0 r2
    FMUL = 0x8A        # multiply two floats and store in first reg e.g.: mulf r0 r2
    DIV = 0x8B         # divide and store in first reg e.g.: div r0 r2
    IDIV = 0x8C        # divide and store in first reg e.g.: div r0 r2
    FDIV = 0x8D        # divide two floats and store in first reg e.g.: divf r0 r2
    SHL = 0x8E         # logical shift left, e.g.: shl r0, r1
    SHR = 0x8F         # logical shift right, e.g.: shr r0, r1
    ISHR = 0x90        # arithmetic shift right (for signed values), e.g.: ishr r0, r1
    MOD = 0x91         # store division remainder in first reg e.g.: mod r0 r1
    IMOD = 0x92        # store signed division remainder in first reg, e.g.: mod r0, r1 
    AND = 0x93         # and two registers and store result in the first one e.g.: and r0 r1
    OR = 0x94          # or two registers and store result in the first one e.g.: or r0 r1
    XOR = 0x95         # xor two registers and store result in the first one e.g.: xor r0 r1
    NOT = 0x96         # not a register and store result e.g.: not r0
    # conversions:
    U2I = 0xA0        # convert an integer stored in a register to a float e.g.: i2f r0
    I2U = 0xA1        # convert a float stored in a register to an int e.g.: f2i r0
    I2F = 0xA2        # convert an integer stored in a register to a float e.g.: i2f r0
    F2I = 0xA3        # convert a float stored in a register to an int e.g.: f2i r0
    # branching:
    JMP = 0xB0        # jump to address e.g.: jmp 0x0A 0x00
    JR = 0xB1         # jump to address in register e.g.: jr r1
    JZ = 0xB2         # jump if zero e.g.: jz r0 0x0A 0x00
    JNZ = 0xB3        # jump if not zero e.g.: jnz r0 0x0A 0x00
    JE = 0xB4         # jump if equal e.g. je r0 r1 0x0A 0x00
    JNE = 0xB5        # jump if not equal e.g. jne r0 r1 0x1C 0x00
    JA = 0xB6       
    JG = 0xB7         # jump if greater e.g. jg r0 r1 0x1C 0x00
    JAE = 0xB8
    JGE = 0xB9        # jump if greater or equal e.g. jge r0 r1 0x1C 0x00
    JB = 0xBA
    JL = 0xBB         # jump if less e.g. jl r0 r1 0x1C 0x00
    JBE = 0xBC
    JLE = 0xBD        # jump if less or equal e.g. jle r0 r1 0x1C 0x00
    # strings:
    PRINT = 0xD0
    PRINTI = 0xD1       # print an integer stored in a register e.g.: printi r0
    PRINTF = 0xD2       # print a float stored in a register e.g.: printf r0
    PRINTP = 0xD3       # print a string stored in the memory address pointed by a register e.g.: print r0
    PRINTLN = 0xD4      # print a newline e.g.: println
    I2S = 0xD5          # convert an int stored in a register to a string in a specific address e.g.: i2s r0 0x20 0x00
    S2I = 0xD6          # convert a string at the specified address to an int in a register e.g.: s2i 0x20 0x00 r0
    # Arduino:
    A_DR = 0xE0        # digital read from a pin to a register e.g.: dr r0 0x0A
    A_AR = 0xE1        # analog read from a pin to a register e.g.: ar r0 0x0A
    A_DW = 0xE2        # digital write to a pin (on/off) e.g.: dw 0xA 0x1
    A_AW = 0xE3        # analog write a short value to a pin e.g.: aw 0xA 0xFF 0x00
    A_DWR = 0xE4       # digital write with a value from a register, e.g.: dwr 0xA, r1
    A_AWR = 0xE5       # analog write with a value from a register e.g.: awr 0xA r1
    A_PM = 0xE6        # set pin mode, e.g.: a_pm 0xA, 0x0
    # system:
    SYSCALL = 0xF0
    HALT = 0xFF    # halt execution
