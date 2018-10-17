from enum import IntEnum

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

class AutoNumber(IntEnum):
    def __new__(cls, value=None):
        if value is None:
            if len(cls.__members__) == 0:
                base = -1
            else:
                base = tuple(cls.__members__.values())[-1]
            value = base + 1

        obj = int.__new__(cls, value)
        obj._value_ = value
        return obj

class Opcodes(AutoNumber):
    # system:  
    NOP = () # do nothing
    HALT = () # halt execution
    INT = ()
    # constants:
    LCONS = ()  # store a value in a register = () e.g.: lcons r0 = () 0xA2 0x00 0x00 0x00
    LCONSW = () # store a word value in a register = () e.g.: lconsw r0 = () 0xA2 0x00
    LCONSB = () # store a byte value in a register = () e.g.: lconsb r0 = () 0xA2
    # register operations:
    MOV = () # copy a value between registers = () e.g.: mov r0 = () r2
    # stack:
    PUSH = () # push a register onto the stack = () e.g.: push r0
    POP = ()  # pop the first element of the stack to a register = () e.g.: pop r0
    POP2 = () # pop the two first elements of the stack to registers = () e.g.: pop r0 = () r1
    DUP = ()  # duplicate the last value in the stack = () e.g.: dup
    # functions
    CALL = () # set register RA to the next instruction and jump to subroutine = () e.g.: call 0x10 0x00
    RET = ()  # return to the address of last callee (RA) = () e.g.: ret
    # memory:
    STOR = ()   # copy a value from a register to a heap address = () e.g.: stor 0x08 0x00 = () r0
    STORW = ()  # copy a word value from a register to a heap address = () e.g.: storw 0x08 0x00 = () r0
    STORB = ()  # copy a byte value from a register to a heap address = () e.g.: storb 0x08 0x00 = () r0
    LOAD = ()   # copy a value from a heap address to a register = () e.g.: load r0 = () 0x08 0x00
    LOADW = ()  # copy a word value from a heap address to a register = () e.g.: loadw r0 = () 0x08 0x00
    LOADB = ()  # copy a byte value from a heap address to a register = () e.g.: loadb r0 = () 0x08 0x00
    MEMCPY = () # copy N bytes from one memory address S to another address D = () e.g.: memcpy 0xDD 0xDD = () 0xSS 0xSS = () 0xNN 0xNN
    # arithmetic:
    INC = ()  # increment the specified register = () e.g.: inc r0
    FINC = () # increment a float in the specified register = () e.g.: incf r0
    DEC = ()  # decrement the specified register = () e.g.: dec r0
    FDEC = () # decrement a float in the specified register = () e.g.: decf r0
    ADD = ()  # sum and store in first reg = () e.g.: add r0 = () r2
    FADD = () # sum two floats and store in first reg = () e.g.: addf r0 = () r2
    SUB = ()  # subtract and store in first reg = () e.g.: sub r0 = () r2
    FSUB = () # subtract two floats and store in first reg = () e.g.: subf r0 = () r2
    MUL = ()  # multiply and store in first reg = () e.g.: mul r0 = () r2
    IMUL = () # signed multiply and store in first reg = () e.g.: mul r0 = () r2
    FMUL = () # multiply two floats and store in first reg = () e.g.: mulf r0 = () r2
    DIV = ()  # divide and store in first reg = () e.g.: div r0 = () r2
    IDIV = () # signed divide and store in first reg = () e.g.: div r0 = () r2
    FDIV = () # divide two floats and store in first reg = () e.g.: divf r0 = () r2
    SHL = ()  # logical shift left = () e.g.: shl r0 = () r1
    SHR = ()  # logical shift right = () e.g.: shr r0 = () r1
    ISHR = () # arithmetic shift right (for signed values) = () e.g.: ishr r0 = () r1
    MOD = ()  # store division remainder in first reg = () e.g.: mod r0 = () r1
    IMOD = () # store signed division remainder in first reg = () e.g.: mod r0 = () r1
    AND = ()  # and two registers and store result in the first one = () e.g.: and r0 = () r1
    OR = ()   # or two registers and store result in the first one = () e.g.: or r0 = () r1
    XOR = ()  # xor two registers and store result in the first one = () e.g.: xor r0 = () r1
    NOT = ()  # not a register and store result = () e.g.: not r0
    # conversions:
    U2I = () # convert an unsigned integer to a signed integer = () e.g.: u2i r0
    I2U = () # convert an signed integer to a unsigned integer = () e.g.: i2u r0
    I2F = () # convert an integer stored in a register to a float = () e.g.: i2f r0
    F2I = () # convert a float stored in a register to an int = () e.g.: f2i r0
    # branching:
    JMP = () # jump to address = () e.g.: jmp 0x0A 0x00
    JR = ()  # jump to address in register = () e.g.: jr r1
    JZ = ()  # jump if zero = () e.g.: jz r0 = () 0x0A 0x00
    JNZ = () # jump if not zero = () e.g.: jnz r0 = () 0x0A 0x00
    JE = ()  # jump if equal = () e.g. je r0 = () r1 = () 0x0A 0x00
    JNE = () # jump if not equal = () e.g. jne r0 = () r1 = () 0x1C 0x00
    JA = ()  # (unsigned) jump if above = () e.g. ja r0 = () r1 = () 0x1C 0x00
    JG = ()  # (signed) jump if greater = () e.g. jg r0 = () r1 = () 0x1C 0x00
    JAE = () # (unsigned) jump if above or equal = () e.g. jae r0 = () r1 = () 0x1C 0x00
    JGE = () # (signed) jump if greater or equal = () e.g. jge r0 = () r1 = () 0x1C 0x00
    JB = ()  # (unsigned) jump if below = () e.g. jb r0 = () r1 = () 0x1C 0x00
    JL = ()  # (signed) jump if less = () e.g. jl r0 = () r1 = () 0x1C 0x00
    JBE = () # (unsigned) jump if below or equal = () e.g. jbe r0 = () r1 = () 0x1C 0x00
    JLE = () # (signed) jump if less or equal = () e.g. jle r0 = () r1 = () 0x1C 0x00
    # strings:
    PRINT = ()   # print an integer stored in a register = () e.g.: print r0
    PRINTI = ()  # print a signed integer stored in a register = () e.g.: printi r0
    PRINTF = ()  # print a float stored in a register = () e.g.: printf r0
    PRINTC = ()  # print a single character from a register
    PRINTS = ()  # print a string stored in a memory address
    PRINTLN = () # print a newline = () e.g.: println
    READ = ()    # read an integer from stdin
    READI = ()   # read a signed integer from stdin to the specified register
    READF = ()   # read a float from stdin to the specified register
    READC = ()   # read a single character's code from stdin to the specified register
    READS = ()   # read a line to the specified memory address, to a maximum length
    I2S = ()     # convert an int stored in a register to a string in a specific address = () e.g.: i2s 0x20 0x00 = () r0
    S2I = ()     # convert a string at the specified address to an int in a register = () e.g.: s2i r0 = () 0x20 0x00
    # Arduino:
    # TODO: pinMode()
    A_DR = ()  # digital read from a pin to a register = () e.g.: dr r0 = () 0x0A
    A_AR = ()  # analog read from a pin to a register = () e.g.: ar r0 = () 0x0A
    A_DW = ()  # digital write to a pin (on/off) = () e.g.: dw 0xA = () 0x1
    A_AW = ()  # analog write a short value to a pin = () e.g.: aw 0xA = () 0xFF 0x00
    A_DWR = () # digital write with a value from a register = () e.g.: dwr 0xA = () r1
    A_AWR = () # analog write with a value from a register = () e.g.: awr 0xA = () r1
    A_PM = ()  # set pin mode = () e.g.: a_pm 0xA = () 0x0
