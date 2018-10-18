#include "vm.h"

#define _NEXT_BYTE this->_program[++this->_registers[IP]]
#define _NEXT_SHORT ({ this->_registers[IP] += 2; this->_program[this->_registers[IP]-1]\
                     | this->_program[this->_registers[IP]] << 8; })
#define _NEXT_INT ({                                                                                 \
    this->_registers[IP] += 4;                                                                       \
    this->_program[this->_registers[IP] - 3] | this->_program[this->_registers[IP] - 2] << 8 |       \
        this->_program[this->_registers[IP] - 1] << 16 | this->_program[this->_registers[IP]] << 24; \
})

VM::VM(uint8_t *program)
    : _program(program)
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
            const uint8_t code = _NEXT_BYTE;

            if (this->_interruptCallback == nullptr)
                return ExecResult::VM_ERR_UNHANDLED_INTERRUPT;
            if (!this->_interruptCallback(code))
                return ExecResult::VM_FINISHED;
            break;
        }
        case OP_MOV:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[reg1] = this->_registers[reg2];
            break;
        }
        case OP_LCONS:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = _NEXT_INT;
            break;
        }
        case OP_LCONSW:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = _NEXT_SHORT;
            break;
        }
        case OP_LCONSB:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = _NEXT_BYTE;
            break;
        }
        case OP_PUSH:
        {
            this->_stack[this->_registers[SP]++] = this->_registers[_NEXT_BYTE];
            break;
        }
        case OP_POP:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = this->_stack[--this->_registers[SP]];
            break;
        }
        case OP_POP2:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[reg1] = this->_stack[--this->_registers[SP]];
            this->_registers[reg2] = this->_stack[--this->_registers[SP]];
            break;
        }
        case OP_DUP:
        {
            this->_stack[this->_registers[SP]] = this->_stack[this->_registers[SP] - 1];
            this->_registers[SP]++;
            break;
        }
        case OP_CALL:
        {
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
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            memcpy(&this->_program[addr], &this->_registers[reg], sizeof(uint32_t));
            break;
        }
        case OP_STORW:
        {
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            memcpy(&this->_program[addr], &this->_registers[reg], sizeof(uint16_t));
            break;
        }
        case OP_STORB:
        {
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            memcpy(&this->_program[addr], &this->_registers[reg], sizeof(uint8_t));
            break;
        }
        case OP_LOAD:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            memcpy(&this->_registers[reg], &this->_program[addr], sizeof(uint32_t));
            break;
        }
        case OP_LOADW:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            this->_registers[reg] = 0;
            memcpy(&this->_registers[reg], &this->_program[addr], sizeof(uint16_t));
            break;
        }
        case OP_LOADB:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            this->_registers[reg] = this->_program[addr];
            break;
        }
        case OP_MEMCPY:
        {
            const uint16_t dest = _NEXT_SHORT;
            const uint16_t source = _NEXT_SHORT;
            const uint16_t bytes = _NEXT_SHORT;
            memcpy((void *)&this->_program[dest], (void *)&this->_program[source], bytes);
            break;
        }
        case OP_INC:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg]++;
            break;
        }
        case OP_FINC:
        {
            const uint8_t reg = _NEXT_BYTE;
            (*((float *)&this->_registers[reg]))++;
            break;
        }
        case OP_DEC:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg]--;
            break;
        }
        case OP_FDEC:
        {
            const uint8_t reg = _NEXT_BYTE;
            (*((float *)&this->_registers[reg]))--;
            break;
        }
        case OP_ADD:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] + this->_registers[reg2];
            break;
        }
        case OP_FADD:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) + *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_SUB:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] - this->_registers[reg2];
            break;
        }
        case OP_FSUB:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) - *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_MUL:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] * this->_registers[reg2];
            break;
        }
        case OP_IMUL:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) * *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_FMUL:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) * *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_DIV:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] / this->_registers[reg2];
            break;
        }
        case OP_IDIV:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) / *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_FDIV:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) / *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_SHL:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] << this->_registers[reg2];
            break;
        }
        case OP_SHR:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] >> this->_registers[reg2];
            break;
        }
        case OP_ISHR:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) >> *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_MOD:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] % this->_registers[reg2];
            break;
        }
        case OP_IMOD:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) % *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_AND:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] & this->_registers[reg2];
            break;
        }
        case OP_OR:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] | this->_registers[reg2];
            break;
        }
        case OP_XOR:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] ^ this->_registers[reg2];
            break;
        }
        case OP_NOT:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            this->_registers[rreg] = ~this->_registers[reg1];
            break;
        }
        case OP_U2I:
        {
            const uint8_t reg = _NEXT_BYTE;
            *((int32_t *)&this->_registers[reg]) = this->_registers[reg];
            break;
        }
        case OP_I2U:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = *((int32_t *)&this->_registers[reg]);
            break;
        }
        case OP_I2F:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            *((float *)&this->_registers[reg]) = (float)*((int32_t *)&this->_registers[reg1]);
            break;
        }
        case OP_F2I:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[reg]) = (int32_t) * ((float *)&this->_registers[reg1]);
            break;
        }
        case OP_JMP:
        {
            this->_registers[IP] = _NEXT_SHORT - 1;
            break;
        }
        case OP_JR:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[IP] = this->_registers[reg] - 1;
            break;
        }
        case OP_JZ:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg] == 0)
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JNZ:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg] != 0)
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] == this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JNE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] != this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JA:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] > this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JG:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (*((int32_t *)&this->_registers[reg1]) > *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JAE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] >= this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JGE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (*((int32_t *)&this->_registers[reg1]) >= *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JB:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] < this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JL:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (*((int32_t *)&this->_registers[reg1]) < *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JBE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] <= this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JLE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (*((int32_t *)&this->_registers[reg1]) <= *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_PRINT:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t ln = _NEXT_BYTE;

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
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t ln = _NEXT_BYTE;

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
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t ln = _NEXT_BYTE;

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
            char* c = (char*)&this->_registers[_NEXT_BYTE];
#ifdef ARDUINO
            Serial.print(*c);
#else
            putchar(*c);
#endif
            break;
        }
        case OP_PRINTS:
        {
            const uint16_t addr = _NEXT_SHORT;
            char *curChar = (char *)&this->_program[addr];
#ifdef ARDUINO
            Serial.print(curChar);
#else
            while (*curChar != '\0')
            {
                putchar(*curChar);
                curChar++;
            }
#endif
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
            const uint8_t reg = _NEXT_BYTE;
#ifdef ARDUINO

#else
            scanf("%u", &this->_registers[reg]);
#endif
            break;
        }
        case OP_READI:
        {
            const uint8_t reg = _NEXT_BYTE;
#ifdef ARDUINO

#else
            scanf("%d", (int32_t *)&this->_registers[reg]);
#endif
            break;
        }
        case OP_READF:
        {
            const uint8_t reg = _NEXT_BYTE;
#ifdef ARDUINO

#else
            scanf("%f", (float *)&this->_registers[reg]);
#endif
            break;
        }
        case OP_READC:
        {
            const uint8_t reg = _NEXT_BYTE;
#ifdef ARDUINO

#else
            this->_registers[reg] = getchar();
#endif
            break;
        }
        case OP_READS:
        {
            char *dest = (char *)&this->_program[_NEXT_SHORT];
            size_t maxLen = _NEXT_SHORT;
#ifdef ARDUINO

#else
            getline(&dest, &maxLen, stdin);
#endif
            break;
        }
        case OP_I2S:
        {
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            sprintf((char *)&this->_program[addr], "%d", this->_registers[reg]);
            break;
        }
        case OP_S2I:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            sscanf((char *)&this->_program[addr], "%d", &this->_registers[reg]);
            break;
        }
#ifdef ARDUINO
        case OP_A_DR:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t pin = _NEXT_BYTE;
            this->_registers[reg] = digitalRead(pin);
            break;
        }
        case OP_A_AR:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t pin = _NEXT_BYTE;
            this->_registers[reg] = analogRead(pin);
            break;
        }
        case OP_A_DW:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t state = _NEXT_BYTE;
            digitalWrite(pin, state);
            break;
        }
        case OP_A_AW:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint16_t val = _NEXT_SHORT;
            analogWrite(pin, val);
            break;
        }
        case OP_A_DWR:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t reg = _NEXT_BYTE;
            digitalWrite(pin, this->_registers[reg]);
            break;
        }
        case OP_A_AWR:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t reg = _NEXT_BYTE;
            analogWrite(pin, this->_registers[reg]);
            break;
        }
        case OP_A_PM:
        {
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