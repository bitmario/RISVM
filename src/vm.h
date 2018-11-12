#ifndef __VM_H__
#define __VM_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#ifdef ARDUINO
#include <Arduino.h>
#endif

enum ExecResult : uint8_t
{
    VM_FINISHED,                // execution completed (i.e. got halt instruction)
    VM_PAUSED,                  // execution paused since we hit the maximum instructions
    VM_ERR_UNKNOWN_OPCODE,      // unknown opcode
    VM_ERR_UNSUPPORTED_OPCODE,  // instruction not supported on this platform
    VM_ERR_INVALID_REGISTER,    // invalid register access
    VM_ERR_UNHANDLED_INTERRUPT, // interrupt triggered without registered handler
    VM_ERR_STACK_OVERFLOW,      // stack overflow
    VM_ERR_STACK_UNDERFLOW,     // stack underflow
    VM_ERR_INVALID_ADDRESS,     // tried to access an invalid memory address
};

enum Instruction : uint8_t
{
    // system:
    OP_NOP,  // do nothing
    OP_HALT, // halt execution
    OP_INT,  // interrupt which should be handled by user-registered function
    // constants:
    OP_LCONS,  // store a value in a register, e.g.: lcons r0, 0xA2 0x00 0x00 0x00
    OP_LCONSW, // store a word value in a register, e.g.: lconsw r0, 0xA2 0x00
    OP_LCONSB, // store a byte value in a register, e.g.: lconsb r0, 0xA2
    // register operations:
    OP_MOV, // copy a value between registers, e.g.: mov r0, r2
    // stack:
    OP_PUSH, // push a register onto the stack, e.g.: push r0
    OP_POP,  // pop the first element of the stack to a register, e.g.: pop r0
    OP_POP2, // pop the two first elements of the stack to registers, e.g.: pop r0, r1
    OP_DUP,  // duplicate the last value in the stack, e.g.: dup
    // functions
    OP_CALL, // set register RA to the next instruction and jump to subroutine, e.g.: call 0x10 0x00
    OP_RET,  // return to the address of last callee (RA), e.g.: ret
    // memory:
    OP_STOR,   // copy a value from a register to a heap address, e.g.: stor 0x08 0x00, r0
    OP_STOR_P,
    OP_STORW,  // copy a word value from a register to a heap address, e.g.: storw 0x08 0x00, r0
    OP_STORW_P,
    OP_STORB,  // copy a byte value from a register to a heap address, e.g.: storb 0x08 0x00, r0
    OP_STORB_P,
    OP_LOAD,   // copy a value from a heap address to a register, e.g.: load r0, 0x08 0x00
    OP_LOAD_P,
    OP_LOADW,  // copy a word value from a heap address to a register, e.g.: loadw r0, 0x08 0x00
    OP_LOADW_P,
    OP_LOADB,  // copy a byte value from a heap address to a register, e.g.: loadb r0, 0x08 0x00
    OP_LOADB_P,
    OP_MEMCPY, // copy N bytes from one memory address S to another address D, e.g.: memcpy 0xDD 0xDD, 0xSS 0xSS, 0xNN 0xNN
    OP_MEMCPY_P,
    // arithmetic:
    OP_INC,  // increment the specified register, e.g.: inc r0
    OP_FINC, // increment a float in the specified register, e.g.: incf r0
    OP_DEC,  // decrement the specified register, e.g.: dec r0
    OP_FDEC, // decrement a float in the specified register, e.g.: decf r0
    OP_ADD,  // sum and store in first reg, e.g.: add r0, r2
    OP_FADD, // sum two floats and store in first reg, e.g.: addf r0, r2
    OP_SUB,  // subtract and store in first reg, e.g.: sub r0, r2
    OP_FSUB, // subtract two floats and store in first reg, e.g.: subf r0, r2
    OP_MUL,  // multiply and store in first reg, e.g.: mul r0, r2
    OP_IMUL, // signed multiply and store in first reg, e.g.: mul r0, r2
    OP_FMUL, // multiply two floats and store in first reg, e.g.: mulf r0, r2
    OP_DIV,  // divide and store in first reg, e.g.: div r0, r2
    OP_IDIV, // signed divide and store in first reg, e.g.: div r0, r2
    OP_FDIV, // divide two floats and store in first reg, e.g.: divf r0, r2
    OP_SHL,  // logical shift left, e.g.: shl r0, r1
    OP_SHR,  // logical shift right, e.g.: shr r0, r1
    OP_ISHR, // arithmetic shift right (for signed values), e.g.: ishr r0, r1
    OP_MOD,  // store in first reg the modulo of rreg 2 by reg 3, e.g.: mod r0, r1, r2
    OP_IMOD, // store signed division remainder in first reg, e.g.: mod r0, r1
    OP_AND,  // and two registers and store result in the first one, e.g.: and r0, r1
    OP_OR,   // or two registers and store result in the first one, e.g.: or r0, r1
    OP_XOR,  // xor two registers and store result in the first one, e.g.: xor r0, r1
    OP_NOT,  // not a register and store result, e.g.: not r0
    // conversions:
    OP_U2I, // convert an unsigned integer to a signed integer, e.g.: u2i r0
    OP_I2U, // convert an signed integer to a unsigned integer, e.g.: i2u r0
    OP_I2F, // convert an integer stored in a register to a float, e.g.: i2f r0
    OP_F2I, // convert a float stored in a register to an int, e.g.: f2i r0
    // branching:
    OP_JMP, // jump to address, e.g.: jmp 0x0A 0x00
    OP_JR,  // jump to address in register, e.g.: jr r1
    OP_JZ,  // jump if zero, e.g.: jz r0, 0x0A 0x00
    OP_JNZ, // jump if not zero, e.g.: jnz r0, 0x0A 0x00
    OP_JE,  // jump if equal, e.g. je r0, r1, 0x0A 0x00
    OP_JNE, // jump if not equal, e.g. jne r0, r1, 0x1C 0x00
    OP_JA,  // (unsigned) jump if above, e.g. ja r0, r1, 0x1C 0x00
    OP_JG,  // (signed) jump if greater, e.g. jg r0, r1, 0x1C 0x00
    OP_JAE, // (unsigned) jump if above or equal, e.g. jae r0, r1, 0x1C 0x00
    OP_JGE, // (signed) jump if greater or equal, e.g. jge r0, r1, 0x1C 0x00
    OP_JB,  // (unsigned) jump if below, e.g. jb r0, r1, 0x1C 0x00
    OP_JL,  // (signed) jump if less, e.g. jl r0, r1, 0x1C 0x00
    OP_JBE, // (unsigned) jump if below or equal, e.g. jbe r0, r1, 0x1C 0x00
    OP_JLE, // (signed) jump if less or equal, e.g. jle r0, r1, 0x1C 0x00
    // strings:
    OP_PRINT,   // print an integer stored in a register, e.g.: print r0
    OP_PRINTI,  // print a signed integer stored in a register, e.g.: printi r0
    OP_PRINTF,  // print a float stored in a register, e.g.: printf r0
    OP_PRINTC,  // print a single char stored in a register
    OP_PRINTS,  // print a string stored in a memory address
    OP_PRINTLN, // print a newline, e.g.: println
    OP_READ,    // read an integer from stdin
    OP_READI,   // read a signed integer from stdin to the specified register
    OP_READF,   // read a float from stdin to the specified register
    OP_READC,   // read a single character's code from stdin to the specified register
    OP_READS,   // read a line to the specified memory address, to a maximum length
#ifdef ARDUINO
    // Arduino:
    OP_A_DR,  // digital read from a pin to a register, e.g.: dr r0, 0x0A
    OP_A_AR,  // analog read from a pin to a register, e.g.: ar r0, 0x0A
    OP_A_DW,  // digital write to a pin (on/off), e.g.: dw 0xA, 0x1
    OP_A_AW,  // analog write a short value to a pin, e.g.: aw 0xA, 0xFF 0x00
    OP_A_DWR, // digital write with a value from a register, e.g.: dwr 0xA, r1
    OP_A_AWR, // analog write with a value from a register, e.g.: awr 0xA, r1
    OP_A_PM,  // set pin mode, e.g.: a_pm 0xA, 0x0
#endif
    INSTRUCTION_COUNT
};

enum Register : uint8_t
{
    // preserved across a call
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    // temporaries - not preserved
    T0,
    T1,
    T2,
    T3,
    T4,
    T5,
    T6,
    T7,
    T8,
    T9,
    // special
    IP,
    BP,
    SP,
    RA,
    // count
    REGISTER_COUNT
};

class VM
{
  public:
    VM(uint8_t *program, uint16_t progLen, uint16_t stackSize = 256);
    ~VM();

    ExecResult run(uint32_t maxInstr = 0);
    void reset();
    void onInterrupt(bool (*callback)(uint8_t));

    uint32_t stackCount();
    void stackPush(uint32_t value);
    uint32_t stackPop();

    uint8_t *memory(uint16_t addr = 0);

    uint32_t getRegister(Register reg);
    void setRegister(Register reg, uint32_t val);

  protected:
    uint8_t *_memory;
    uint32_t _registers[REGISTER_COUNT] = {0};
    const uint16_t _memSize;
    const uint16_t _stackSize;
    const uint16_t _progLen;
    bool (*_interruptCallback)(uint8_t) = nullptr;
};

#endif // __VM_H__