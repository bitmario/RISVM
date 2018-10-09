#ifndef __VM_H__
#define __VM_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define VM_STACK_SIZE 128

enum Instructions
{
    OP_NOP = 0x00, // do nothing
    // constants:
    OP_LCONS = 0x10, // store a value in a register, e.g.: lcons ax, 0xA2 0x00 0x00 0x00
    OP_LCONSW,       // store a word value in a register, e.g.: lconsw ax, 0xA2 0x00
    OP_LCONSB,       // store a byte value in a register, e.g.: lconsb ax, 0xA2
    // register operations:
    OP_MOV = 0x30, // copy a value between registers, e.g.: mov ax, cx
    // stack:
    OP_PUSH = 0x40, // push a register onto the stack, e.g.: push ax
    OP_POP,         // pop the first element of the stack to a register, e.g.: pop ax
    OP_POP2,        // pop the two first elements of the stack to registers, e.g.: pop ax, bx
    OP_DUP,         // duplicate the last value in the stack, e.g.: dup
    // memory:
    OP_STOR = 0x60, // copy a value from a register to a heap address, e.g.: stor 0x08 0x00, ax
    OP_STORW,       // copy a word value from a register to a heap address, e.g.: storw 0x08 0x00, ax
    OP_STORB,       // copy a byte value from a register to a heap address, e.g.: storb 0x08 0x00, ax
    OP_CSTORS,      // load a constant null-terminated string to the specified memory address. e.g.: cstors 0x10, 0x00, 'H' 'i' '!' 0x00
    OP_CSTORS_R,    // load a constant null-terminated string to a memory address in a register. e.g.: cstors_r ax, 'H' 'i' '!' 0x00
    OP_LOAD,        // copy a value from a heap address to a register, e.g.: load ax, 0x08 0x00
    OP_LOADW,       // copy a word value from a heap address to a register, e.g.: loadw ax, 0x08 0x00
    OP_LOADB,       // copy a byte value from a heap address to a register, e.g.: loadb ax, 0x08 0x00
    OP_MEMCPY,      // copy N bytes from one memory address S to another address D, e.g.: memcpy 0xDD 0xDD, 0xSS 0xSS, 0xNN 0xNN
    // arithmetic:
    // TODO: add signed/unsigned operators and logical/arithmetic shifts
    OP_INC = 0x80, // increment the specified register, e.g.: inc ax
    OP_INCF,       // increment a float in the specified register, e.g.: incf ax
    OP_DEC,        // decrement the specified register, e.g.: dec ax
    OP_DECF,       // decrement a float in the specified register, e.g.: decf ax
    OP_ADD,         // sum and store in first reg, e.g.: add ax, cx
    OP_ADDF,        // sum two floats and store in first reg, e.g.: addf ax, cx
    OP_SUB,         // subtract and store in first reg, e.g.: sub ax, cx
    OP_SUBF,        // subtract two floats and store in first reg, e.g.: subf ax, cx
    OP_MUL,         // multiply and store in first reg, e.g.: mul ax, cx
    OP_MULF,        // multiply two floats and store in first reg, e.g.: mulf ax, cx
    OP_DIV,         // divide and store in first reg, e.g.: div ax, cx
    OP_DIVF,        // divide two floats and store in first reg, e.g.: divf ax, cx
    OP_MOD,         // store division remainder in first reg, e.g.: mod ax, bx
    OP_AND,         // and two registers and store result in the first one, e.g.: and ax, bx
    OP_OR,          // or two registers and store result in the first one, e.g.: or ax, bx
    OP_XOR,         // xor two registers and store result in the first one, e.g.: xor ax, bx
    OP_NOT,         // not a register and store result, e.g.: not ax
    // conversions:
    OP_I2F = 0xA0, // convert an integer stored in a register to a float, e.g.: i2f ax
    OP_F2I,        // convert a float stored in a register to an int, e.g.: f2i ax
    // branching:
    OP_JMP = 0xB0, // jump to address, e.g.: jmp 0x0A 0x00
    OP_JR,         // jump to address in register, e.g.: jr bx
    OP_JZ,         // jump if zero, e.g.: jz ax, 0x0A 0x00
    OP_JNZ,        // jump if not zero, e.g.: jnz ax, 0x0A 0x00
    OP_JGZ,        // jump if greater than zero, e.g.: jgz ax, 0x0A 0x00
    OP_JLZ,        // jump if less than zero, e.g.: jlz ax, 0x0A 0x00
    OP_JE,         // jump if equal, e.g. je ax, bx, 0x0A 0x00
    OP_JNE,        // jump if not equal, e.g. jne ax, bx, 0x1C 0x00
    OP_JG,         // jump if greater, e.g. jg ax, bx, 0x1C 0x00
    OP_JGE,        // jump if greater or equal, e.g. jge ax, bx, 0x1C 0x00
    OP_JL,         // jump if less, e.g. jl ax, bx, 0x1C 0x00
    OP_JLE,        // jump if less or equal, e.g. jle ax, bx, 0x1C 0x00
    // strings:
    OP_PRINT = 0xD0, // print a string stored in the memory address pointed by a register, e.g.: print ax
    OP_PRINTI,       // print an integer stored in a register, e.g.: printi ax
    OP_PRINTF,       // print a float stored in a register, e.g.: printf ax
    OP_PRINTLN,      // print a newline, e.g.: println
    OP_I2S,          // convert an int stored in a register to a string in a specific address, e.g.: i2s 0x20 0x00, ax
    OP_S2I,          // convert a string at the specified address to an int in a register, e.g.: s2i ax, 0x20 0x00
    // Arduino:
    // TODO: pinMode()
    OP_A_DR = 0xE0, // digital read from a pin to a register, e.g.: dr ax, 0x0A
    OP_A_AR,        // analog read from a pin to a register, e.g.: ar ax, 0x0A
    OP_A_DW,        // digital write to a pin (on/off), e.g.: dw 0xA, 0x1
    OP_A_AW,        // analog write a short value to a pin, e.g.: aw 0xA, 0xFF 0x00
    OP_A_DWR,       // digital write with a value from a register, e.g.: dwr 0xA, bx
    OP_A_AWR,       // analog write with a value from a register, e.g.: awr 0xA, bx
    OP_A_PM,        // set pin mode, e.g.: a_pm 0xA, 0x0
    // system:
    OP_SYSCALL = 0xF0,
    OP_HALT = 0xFF // halt execution
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
    VM(const uint8_t *program, const uint8_t *data, uint16_t dataSize, uint16_t zeroBytes = 0);
    ~VM();
    void run();

  protected:
    const uint8_t *_program;
    uint8_t *_data;
    uint32_t _stack[VM_STACK_SIZE] = {0};
    uint32_t _registers[REGISTERS_MAX] = {0};
    uint16_t _dataSize = 0;
    bool _running = false;

    void _eval();
};

#endif // __VM_H__