from enum import IntEnum

labels = {}
label_instances = {}

REGISTERS = {
    "ax": 0,
    "bx": 1,
    "cx": 2,
    "dx": 3,
    "ex": 4,
    "fx": 5,
    "ip": 6,
    "bp": 7,
    "sp": 8
}

class Opcodes(IntEnum):
    NOP = 0x00 # do nothing
    # constants:
    LCONS = 0x10   # store a value in a register e.g.: lcons ax 0xA2 0x00 0x00 0x00
    LCONSW = 0x11  # store a word value in a register e.g.: lconsw ax 0xA2 0x00
    LCONSB = 0x12  # store a byte value in a register e.g.: lconsb ax 0xA2
    # register operations:
    MOV = 0x30     # copy a value between registers e.g.: mov ax cx
    # stack:
    PUSH = 0x40        # push a register onto the stack e.g.: push ax
    POP = 0x41         # pop the first element of the stack to a register e.g.: pop ax
    POP2 = 0x42        # pop the two first elements of the stack to registers e.g.: pop ax bx
    DUP = 0x43         # duplicate the last value in the stack e.g.: dup
    # memory:
    STOR = 0x60        # copy a value from a register to a heap address e.g.: stor 0x08 0x00 ax
    STORW = 0x61       # copy a word value from a register to a heap address e.g.: storw 0x08 0x00 ax
    STORB = 0x62       # copy a byte value from a register to a heap address e.g.: storb 0x08 0x00 ax
    CSTORS = 0x63      # load a constant null-terminated string to the specified memory address. e.g.: cstors 0x10 0x00 'H' 'i' '!' 0x00
    CSTORS_R = 0x64    # load a constant null-terminated string to a memory address in a register. e.g.: cstors_r ax 'H' 'i' '!' 0x00
    LOAD = 0x65        # copy a value from a heap address to a register e.g.: load ax 0x08 0x00
    LOADW = 0x66       # copy a word value from a heap address to a register e.g.: loadw ax 0x08 0x00
    LOADB = 0x67       # copy a byte value from a heap address to a register e.g.: loadb ax 0x08 0x00
    MEMCPY = 0x68      # copy N bytes from one memory address S to another address D e.g.: memcpy 0xDD 0xDD 0xSS 0xSS 0xNN 0xNN
    # arithmetic:
    INC = 0x80         # increment the specified register e.g.: inc ax
    INCF = 0x81        # increment a float in the specified register e.g.: incf ax
    DEC = 0x82         # decrement the specified register e.g.: dec ax
    DECF = 0x83        # decrement a float in the specified register e.g.: decf ax
    ADD = 0x84         # sum and store in first reg e.g.: add ax cx
    ADDF = 0x85        # sum two floats and store in first reg e.g.: addf ax cx
    SUB = 0x86         # subtract and store in first reg e.g.: sub ax cx
    SUBF = 0x87        # subtract two floats and store in first reg e.g.: subf ax cx
    MUL = 0x88         # multiply and store in first reg e.g.: mul ax cx
    IMUL = 0x89        # multiply and store in first reg e.g.: mul ax cx
    FMUL = 0x8A        # multiply two floats and store in first reg e.g.: mulf ax cx
    DIV = 0x8B         # divide and store in first reg e.g.: div ax cx
    IDIV = 0x8C        # divide and store in first reg e.g.: div ax cx
    FDIV = 0x8D        # divide two floats and store in first reg e.g.: divf ax cx
    SHL = 0x8E         # logical shift left, e.g.: shl ax, bx
    SHR = 0x8F         # logical shift right, e.g.: shr ax, bx
    ISHR = 0x90        # arithmetic shift right (for signed values), e.g.: ishr ax, bx
    MOD = 0x91         # store division remainder in first reg e.g.: mod ax bx
    AND = 0x92         # and two registers and store result in the first one e.g.: and ax bx
    OR = 0x93          # or two registers and store result in the first one e.g.: or ax bx
    XOR = 0x94         # xor two registers and store result in the first one e.g.: xor ax bx
    NOT = 0x95         # not a register and store result e.g.: not ax
    # conversions:
    U2I = 0xA0        # convert an integer stored in a register to a float e.g.: i2f ax
    I2U = 0xA1        # convert a float stored in a register to an int e.g.: f2i ax
    I2F = 0xA2        # convert an integer stored in a register to a float e.g.: i2f ax
    F2I = 0xA3        # convert a float stored in a register to an int e.g.: f2i ax
    # branching:
    JMP = 0xB0        # jump to address e.g.: jmp 0x0A 0x00
    JR = 0xB1         # jump to address in register e.g.: jr bx
    JZ = 0xB2         # jump if zero e.g.: jz ax 0x0A 0x00
    JNZ = 0xB3        # jump if not zero e.g.: jnz ax 0x0A 0x00
    JE = 0xB4         # jump if equal e.g. je ax bx 0x0A 0x00
    JNE = 0xB5        # jump if not equal e.g. jne ax bx 0x1C 0x00
    JA = 0xB6       
    JG = 0xB7         # jump if greater e.g. jg ax bx 0x1C 0x00
    JAE = 0xB8
    JGE = 0xB9        # jump if greater or equal e.g. jge ax bx 0x1C 0x00
    JB = 0xBA
    JL = 0xBB         # jump if less e.g. jl ax bx 0x1C 0x00
    JBE = 0xBC
    JLE = 0xBD        # jump if less or equal e.g. jle ax bx 0x1C 0x00
    # strings:
    PRINT = 0xD0
    PRINTI = 0xD1       # print an integer stored in a register e.g.: printi ax
    PRINTF = 0xD2       # print a float stored in a register e.g.: printf ax
    PRINTP = 0xD3       # print a string stored in the memory address pointed by a register e.g.: print ax
    PRINTLN = 0xD4      # print a newline e.g.: println
    I2S = 0xD5          # convert an int stored in a register to a string in a specific address e.g.: i2s ax 0x20 0x00
    S2I = 0xD6          # convert a string at the specified address to an int in a register e.g.: s2i 0x20 0x00 ax
    # Arduino:
    A_DR = 0xE0        # digital read from a pin to a register e.g.: dr ax 0x0A
    A_AR = 0xE1        # analog read from a pin to a register e.g.: ar ax 0x0A
    A_DW = 0xE2        # digital write to a pin (on/off) e.g.: dw 0xA 0x1
    A_AW = 0xE3        # analog write a short value to a pin e.g.: aw 0xA 0xFF 0x00
    A_DWR = 0xE4       # digital write with a value from a register, e.g.: dwr 0xA, bx
    A_AWR = 0xE5       # analog write with a value from a register e.g.: awr 0xA bx
    A_PM = 0xE6        # set pin mode, e.g.: a_pm 0xA, 0x0
    # system:
    SYSCALL = 0xF0
    HALT = 0xFF    # halt execution
