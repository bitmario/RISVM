#include "vm.h"

#define _NEXT_BYTE this->_program[++this->_registers[IP]]
#define _NEXT_SHORT ({ this->_registers[IP] += 2; this->_program[this->_registers[IP]-1]\
                     | this->_program[this->_registers[IP]] << 8; })
#define _NEXT_INT ({                                                                                 \
    this->_registers[IP] += 4;                                                                       \
    this->_program[this->_registers[IP] - 3] | this->_program[this->_registers[IP] - 2] << 8 |       \
        this->_program[this->_registers[IP] - 1] << 16 | this->_program[this->_registers[IP]] << 24; \
})

#ifndef VM_DISABLE_CHECKS
#define _CHECK_ADDR_VALID(a) \
    if (a >= this->_progLen) \
        return ExecResult::VM_ERR_PROGRAM_OVERRUN;
#define _CHECK_BYTES_AVAIL(n) \
    _CHECK_ADDR_VALID(this->_registers[IP] + n)
#define _CHECK_REGISTER_VALID(r) \
    if (r >= REGISTER_COUNT)     \
        return ExecResult::VM_ERR_INVALID_REGISTER;
#define _CHECK_CAN_PUSH(n)                        \
    if (this->_registers[SP] + n > VM_STACK_SIZE) \
        return ExecResult::VM_ERR_STACK_OVERFLOW;
#define _CHECK_CAN_POP(n)                          \
    if (this->_registers[SP] < n)                  \
        return ExecResult::VM_ERR_STACK_UNDERFLOW; \
    if (this->_registers[SP] > VM_STACK_SIZE)      \
        return ExecResult::VM_ERR_STACK_OVERFLOW;
#else
#define _CHECK_ADDR_VALID(a)
#define _CHECK_BYTES_AVAIL(n)
#define _CHECK_REGISTER_VALID(r)
#define _CHECK_CAN_PUSH(n)
#define _CHECK_CAN_POP(n)
#endif

VM::VM(uint8_t *program, uint16_t progLen)
    : _program(program), _progLen(progLen)
{
}

VM::~VM()
{
}

ExecResult VM::run(uint32_t maxInstr)
{
    uint32_t instrCount = 0;

    while (instrCount < maxInstr)
    {
        _CHECK_ADDR_VALID(this->_registers[IP])
        const uint8_t instr = this->_program[this->_registers[IP]];
        if (instr >= INSTRUCTION_COUNT)
            return ExecResult::VM_ERR_UNKNOWN_OPCODE;

        switch (instr)
        {
        case OP_NOP:
        {
            break;
        }
        case OP_HALT:
        {
            return ExecResult::VM_FINISHED;
        }
        case OP_INT:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t code = _NEXT_BYTE;

            if (this->_interruptCallback == nullptr)
                return ExecResult::VM_ERR_UNHANDLED_INTERRUPT;
            if (!this->_interruptCallback(code))
                return ExecResult::VM_FINISHED;
            break;
        }
        case OP_MOV:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[reg1] = this->_registers[reg2];
            break;
        }
        case OP_LCONS:
        {
            _CHECK_BYTES_AVAIL(5)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[reg] = _NEXT_INT;
            break;
        }
        case OP_LCONSW:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[reg] = _NEXT_SHORT;
            break;
        }
        case OP_LCONSB:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[reg] = _NEXT_BYTE;
            break;
        }
        case OP_PUSH:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_CAN_PUSH(1)
            this->_stack[this->_registers[SP]++] = this->_registers[reg];
            break;
        }
        case OP_POP:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_CAN_POP(1)
            this->_registers[reg] = this->_stack[--this->_registers[SP]];
            break;
        }
        case OP_POP2:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            _CHECK_CAN_POP(2)
            this->_registers[reg1] = this->_stack[--this->_registers[SP]];
            this->_registers[reg2] = this->_stack[--this->_registers[SP]];
            break;
        }
        case OP_DUP:
        {
            _CHECK_CAN_PUSH(1)
            this->_stack[this->_registers[SP]] = this->_stack[this->_registers[SP] - 1];
            this->_registers[SP]++;
            break;
        }
        case OP_CALL:
        {
            _CHECK_BYTES_AVAIL(2)
            this->_registers[RA] = this->_registers[IP] + 3;
            this->_registers[IP] = _NEXT_SHORT - 1;
            break;
        }
        case OP_RET:
        {
            this->_registers[IP] = this->_registers[RA] - 1;
            break;
        }
        case OP_STOR:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_ADDR_VALID((uint32_t)addr + 3)
            memcpy(&this->_program[addr], &this->_registers[reg], sizeof(uint32_t));
            break;
        }
        case OP_STORW:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_ADDR_VALID((uint32_t)addr + 1)
            memcpy(&this->_program[addr], &this->_registers[reg], sizeof(uint16_t));
            break;
        }
        case OP_STORB:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_ADDR_VALID(addr)
            memcpy(&this->_program[addr], &this->_registers[reg], sizeof(uint8_t));
            break;
        }
        case OP_LOAD:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_ADDR_VALID((uint32_t)addr + 3)
            memcpy(&this->_registers[reg], &this->_program[addr], sizeof(uint32_t));
            break;
        }
        case OP_LOADW:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_ADDR_VALID((uint32_t)addr + 1)
            this->_registers[reg] = 0;
            memcpy(&this->_registers[reg], &this->_program[addr], sizeof(uint16_t));
            break;
        }
        case OP_LOADB:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_ADDR_VALID((uint32_t)addr)
            this->_registers[reg] = this->_program[addr];
            break;
        }
        case OP_MEMCPY:
        {
            _CHECK_BYTES_AVAIL(6)
            const uint16_t dest = _NEXT_SHORT;
            const uint16_t source = _NEXT_SHORT;
            const uint16_t bytes = _NEXT_SHORT;
            _CHECK_ADDR_VALID((uint32_t)source + bytes - 1)
            _CHECK_ADDR_VALID((uint32_t)dest + bytes - 1)
            memcpy((void *)&this->_program[dest], (void *)&this->_program[source], bytes);
            break;
        }
        case OP_INC:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[reg]++;
            break;
        }
        case OP_FINC:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            (*((float *)&this->_registers[reg]))++;
            break;
        }
        case OP_DEC:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[reg]--;
            break;
        }
        case OP_FDEC:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            (*((float *)&this->_registers[reg]))--;
            break;
        }
        case OP_ADD:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] + this->_registers[reg2];
            break;
        }
        case OP_FADD:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) + *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_SUB:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] - this->_registers[reg2];
            break;
        }
        case OP_FSUB:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) - *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_MUL:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] * this->_registers[reg2];
            break;
        }
        case OP_IMUL:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) * *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_FMUL:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) * *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_DIV:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] / this->_registers[reg2];
            break;
        }
        case OP_IDIV:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) / *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_FDIV:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) / *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_SHL:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] << this->_registers[reg2];
            break;
        }
        case OP_SHR:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] >> this->_registers[reg2];
            break;
        }
        case OP_ISHR:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) >> *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_MOD:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] % this->_registers[reg2];
            break;
        }
        case OP_IMOD:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) % *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_AND:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] & this->_registers[reg2];
            break;
        }
        case OP_OR:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] | this->_registers[reg2];
            break;
        }
        case OP_XOR:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)
            this->_registers[rreg] = this->_registers[reg1] ^ this->_registers[reg2];
            break;
        }
        case OP_NOT:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(rreg)
            _CHECK_REGISTER_VALID(reg1)
            this->_registers[rreg] = ~this->_registers[reg1];
            break;
        }
        case OP_U2I:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            *((int32_t *)&this->_registers[reg]) = this->_registers[reg];
            break;
        }
        case OP_I2U:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[reg] = *((int32_t *)&this->_registers[reg]);
            break;
        }
        case OP_I2F:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_REGISTER_VALID(reg1)
            *((float *)&this->_registers[reg]) = (float)*((int32_t *)&this->_registers[reg1]);
            break;
        }
        case OP_F2I:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            _CHECK_REGISTER_VALID(reg1)
            *((int32_t *)&this->_registers[reg]) = (int32_t) * ((float *)&this->_registers[reg1]);
            break;
        }
        case OP_JMP:
        {
            _CHECK_BYTES_AVAIL(2)
            this->_registers[IP] = _NEXT_SHORT - 1;
            break;
        }
        case OP_JR:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[IP] = this->_registers[reg] - 1;
            break;
        }
        case OP_JZ:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg)

            if (this->_registers[reg] == 0)
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JNZ:
        {
            _CHECK_BYTES_AVAIL(3)
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg)

            if (this->_registers[reg] != 0)
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JE:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (this->_registers[reg1] == this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JNE:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (this->_registers[reg1] != this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JA:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (this->_registers[reg1] > this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JG:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (*((int32_t *)&this->_registers[reg1]) > *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JAE:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (this->_registers[reg1] >= this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JGE:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (*((int32_t *)&this->_registers[reg1]) >= *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JB:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (this->_registers[reg1] < this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JL:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (*((int32_t *)&this->_registers[reg1]) < *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JBE:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (this->_registers[reg1] <= this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JLE:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_REGISTER_VALID(reg1)
            _CHECK_REGISTER_VALID(reg2)

            if (*((int32_t *)&this->_registers[reg1]) <= *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_PRINT:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t ln = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)

#ifdef ARDUINO
            if (ln == 0)
                Serial.printf("%u", this->_registers[reg]);
            else
                Serial.printf("%u\n", this->_registers[reg]);
#else
            printf("%u", this->_registers[reg]);
            if (ln != 0)
                putchar('\n');
#endif
            break;
        }
        case OP_PRINTI:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t ln = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)

#ifdef ARDUINO
            if (ln == 0)
                Serial.printf("%d", *((int32_t *)&this->_registers[reg]));
            else
                Serial.printf("%d\n", *((int32_t *)&this->_registers[reg]));
#else
            printf("%d", *((int32_t *)&this->_registers[reg]));
            if (ln != 0)
                putchar('\n');
#endif
            break;
        }
        case OP_PRINTF:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t ln = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)

#ifdef ARDUINO
            if (ln == 0)
                Serial.printf("%f", *((float *)&this->_registers[reg]));
            else
                Serial.printf("%f\n", *((float *)&this->_registers[reg]));
#else
            printf("%f", *((float *)&this->_registers[reg]));
            if (ln != 0)
                putchar('\n');
#endif
            break;
        }
        case OP_PRINTC:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            char *c = (char *)&this->_registers[reg];
#ifdef ARDUINO
            Serial.print(*c);
#else
            putchar(*c);
#endif
            break;
        }
        case OP_PRINTS:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint16_t addr = _NEXT_SHORT;
            _CHECK_ADDR_VALID(addr)
            char *curChar = (char *)&this->_program[addr];

            while (*curChar != '\0')
            {
#ifdef ARDUINO
                Serial.print(curChar);
#else
                putchar(*curChar);
#endif
                curChar++;
                _CHECK_ADDR_VALID((uint8_t *)curChar - this->_program)
            }
            break;
        }
        case OP_PRINTLN:
        {
#ifdef ARDUINO
            Serial.println();
#else
            putchar('\n');
#endif
            break;
        }
        case OP_READ:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
#ifdef ARDUINO
            return ExecResult::VM_ERR_UNSUPPORTED_OPCODE;
#else
            scanf("%u", &this->_registers[reg]);
#endif
            break;
        }
        case OP_READI:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
#ifdef ARDUINO
            return ExecResult::VM_ERR_UNSUPPORTED_OPCODE;
#else
            scanf("%d", (int32_t *)&this->_registers[reg]);
#endif
            break;
        }
        case OP_READF:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
#ifdef ARDUINO
            return ExecResult::VM_ERR_UNSUPPORTED_OPCODE;
#else
            scanf("%f", (float *)&this->_registers[reg]);
#endif
            break;
        }
        case OP_READC:
        {
            _CHECK_BYTES_AVAIL(1)
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
#ifdef ARDUINO
            return ExecResult::VM_ERR_UNSUPPORTED_OPCODE;
#else
            this->_registers[reg] = getchar();
#endif
            break;
        }
        case OP_READS:
        {
            _CHECK_BYTES_AVAIL(4)
            const uint16_t addr = _NEXT_SHORT;
            size_t maxLen = _NEXT_SHORT;
            _CHECK_ADDR_VALID((uint32_t)addr + maxLen)
            char *dest = (char *)&this->_program[addr];
#ifdef ARDUINO
            return ExecResult::VM_ERR_UNSUPPORTED_OPCODE;
#else
            getline(&dest, &maxLen, stdin);
#endif
            break;
        }
#ifdef ARDUINO
        case OP_A_DR:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t pin = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[reg] = digitalRead(pin);
            break;
        }
        case OP_A_AR:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t pin = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            this->_registers[reg] = analogRead(pin);
            break;
        }
        case OP_A_DW:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t state = _NEXT_BYTE;
            digitalWrite(pin, state);
            break;
        }
        case OP_A_AW:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t pin = _NEXT_BYTE;
            const uint16_t val = _NEXT_SHORT;
            analogWrite(pin, val);
            break;
        }
        case OP_A_DWR:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            digitalWrite(pin, this->_registers[reg]);
            break;
        }
        case OP_A_AWR:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t reg = _NEXT_BYTE;
            _CHECK_REGISTER_VALID(reg)
            analogWrite(pin, this->_registers[reg]);
            break;
        }
        case OP_A_PM:
        {
            _CHECK_BYTES_AVAIL(2)
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t mode = _NEXT_BYTE;
            pinMode(pin, mode);
            break;
        }
#endif
        }

        this->_registers[IP]++;
        instrCount++;
    }

    return ExecResult::VM_PAUSED;
}