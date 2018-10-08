#ifndef __VM_H__
#define __VM_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define VM_STACK_SIZE 128

enum Instructions
{
    NOP = 0x00, // do nothing
    // constants:
    LCONS = 0x10, // store a value in a register, e.g.: lcons ax, 0xA2 0x00 0x00 0x00
    LCONSW,       // store a word value in a register, e.g.: lconsw ax, 0xA2 0x00
    LCONSB,       // store a byte value in a register, e.g.: lconsb ax, 0xA2
    // register operations:
    MOV = 0x30, // copy a value between registers, e.g.: mov ax, cx
    // stack:
    PUSH = 0x40, // push a register onto the stack, e.g.: push ax
    POP,         // pop the first element of the stack to a register, e.g.: pop ax
    POP2,        // pop the two first elements of the stack to registers, e.g.: pop ax, bx
    DUP,         // duplicate the last value in the stack, e.g.: dup
    // memory:
    STOR = 0x60, // copy a value from a register to a heap address, e.g.: stor 0x08 0x00, ax
    STORW,       // copy a word value from a register to a heap address, e.g.: storw 0x08 0x00, ax
    STORB,       // copy a byte value from a register to a heap address, e.g.: storb 0x08 0x00, ax
    CSTORS,      // load a constant null-terminated string to the specified memory address. e.g.: cstors 0x10, 0x00, 'H' 'i' '!' 0x00
    CSTORS_R,    // load a constant null-terminated string to a memory address in a register. e.g.: cstors_r ax, 'H' 'i' '!' 0x00
    LOAD,        // copy a value from a heap address to a register, e.g.: load ax, 0x08 0x00
    LOADW,       // copy a word value from a heap address to a register, e.g.: loadw ax, 0x08 0x00
    LOADB,       // copy a byte value from a heap address to a register, e.g.: loadb ax, 0x08 0x00
    MEMCPY,      // copy N bytes from one memory address S to another address D, e.g.: memcpy 0xDD 0xDD, 0xSS 0xSS, 0xNN 0xNN
    // arithmetic:
    INCR = 0x80, // increment the specified register, e.g.: incr ax
    INCRF,       // increment a float in the specified register, e.g.: incrf ax
    DECR,        // decrement the specified register, e.g.: decr ax
    DECRF,       // decrement a float in the specified register, e.g.: decrf ax
    ADD,         // sum and store in first reg, e.g.: add ax, cx
    ADDF,        // sum two floats and store in first reg, e.g.: addf ax, cx
    SUB,         // subtract and store in first reg, e.g.: sub ax, cx
    SUBF,        // subtract two floats and store in first reg, e.g.: subf ax, cx
    MULT,        // multiply and store in first reg, e.g.: mult ax, cx
    MULTF,       // multiply two floats and store in first reg, e.g.: multf ax, cx
    DIV,         // divide and store in first reg, e.g.: div ax, cx
    DIVF,        // divide two floats and store in first reg, e.g.: divf ax, cx
    MOD,         // store division remainder in first reg, e.g.: mod ax, bx
    AND,         // and two registers and store result in the first one, e.g.: and ax, bx
    OR,          // or two registers and store result in the first one, e.g.: or ax, bx
    XOR,         // xor two registers and store result in the first one, e.g.: xor ax, bx
    NOT,         // not a register and store result, e.g.: not ax
    // conversions:
    I2F = 0xA0, // convert an integer stored in a register to a float, e.g.: i2f ax
    F2I,        // convert a float stored in a register to an int, e.g.: f2i ax
    // branching:
    JMP = 0xB0, // jump to address, e.g.: jmp 0x0A 0x00
    JR,         // jump to address in register, e.g.: jr bx
    JZ,         // jump if zero, e.g.: jz ax, 0x0A 0x00
    JNZ,        // jump if not zero, e.g.: jnz ax, 0x0A 0x00
    JGZ,        // jump if greater than zero, e.g.: jgz ax, 0x0A 0x00
    JLZ,        // jump if less than zero, e.g.: jlz ax, 0x0A 0x00
    JE,         // jump if equal, e.g. je ax, bx, 0x0A 0x00
    JNE,        // jump if not equal, e.g. jne ax, bx, 0x1C 0x00
    JG,         // jump if greater, e.g. jg ax, bx, 0x1C 0x00
    JGE,        // jump if greater or equal, e.g. jge ax, bx, 0x1C 0x00
    JL,         // jump if less, e.g. jl ax, bx, 0x1C 0x00
    JLE,        // jump if less or equal, e.g. jle ax, bx, 0x1C 0x00
    // strings:
    PRINT = 0xD0, // print a string stored in the memory address pointed by a register, e.g.: print ax
    PRINTI,       // print an integer stored in a register, e.g.: printi ax
    PRINTF,       // print a float stored in a register, e.g.: printf ax
    PRINTLN,      // print a newline, e.g.: println
    I2S,          // convert an int stored in a register to a string in a specific address, e.g.: i2s 0x20 0x00, ax
    S2I,          // convert a string at the specified address to an int in a register, e.g.: s2i ax, 0x20 0x00
    // Arduino:
    // TODO: pinMode()
    A_DR = 0xE0, // digital read from a pin to a register, e.g.: dr ax, 0x0A
    A_AR,        // analog read from a pin to a register, e.g.: ar ax, 0x0A
    A_DW,        // digital write to a pin (on/off), e.g.: dw 0xA, 0x1
    A_AW,        // analog write a short value to a pin, e.g.: aw 0xA, 0xFF 0x00
    A_DWR,       // digital write with a value from a register, e.g.: dwr 0xA, bx
    A_AWR,       // analog write with a value from a register, e.g.: awr 0xA, bx
    A_PM,        // set pin mode, e.g.: a_pm 0xA, 0x0
    // system:
    SYSCALL = 0xF0,
    HALT = 0xFF // halt execution
};

enum Registers
{
    // general purpose
    AX,
    BX,
    CX,
    DX,
    EX,
    FX,
    // base
    IP,
    BP,
    SP,
    // count for our array
    REGISTERS_MAX
};

class VM
{
  public:
    VM(const uint8_t *program, const uint8_t *data, int16_t dataSize, int16_t zeroBytes = 0);
    ~VM();
    void run();

  protected:
    const uint8_t *_program;
    uint8_t *_data;
    int32_t _stack[VM_STACK_SIZE] = {0};
    int32_t _registers[REGISTERS_MAX] = {0};
    int16_t _dataSize = 0;
    bool _running = false;

    void _eval();
};

#endif // __VM_H__