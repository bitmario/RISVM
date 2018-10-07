#include "vm.h"

VM::VM(const uint8_t *program, const uint8_t *data, int16_t dataSize, int16_t freeBytes)
    : _program(program), _dataSize(dataSize + freeBytes)
{
    if (this->_dataSize > 0)
    {
        this->_data = new uint8_t[this->_dataSize];
        if (dataSize > 0)
            memcpy((void *)this->_data, (void *)data, dataSize);
    }
}

VM::~VM()
{
    if (this->_dataSize > 0)
        delete[] this->_data;
}

void VM::run()
{
    this->_running = true;

    while (this->_running)
    {
        this->_eval();
    }

    this->_registers[IP] = 0;
    this->_registers[SP] = 0;
}

void VM::_eval()
{
    switch (this->_program[this->_registers[IP]])
    {
    case MOV:
    {
        const uint8_t reg1 = this->_readByte();
        const uint8_t reg2 = this->_readByte();
        this->_registers[reg1] = this->_registers[reg2];
        break;
    }
    case LCONS:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] = this->_readInt();
        break;
    }
    case LCONSW:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] = this->_readShort();
        break;
    }
    case LCONSB:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] = this->_readByte();
        break;
    }
    case PUSH:
    {
        this->_stack[++this->_registers[SP]] = this->_registers[this->_readByte()];
        break;
    }
    case POP:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] = this->_stack[this->_registers[SP]--];
        printf("popped %d\n", this->_registers[reg]);
        break;
    }
    case POP2:
    {
        const uint8_t reg1 = this->_readByte();
        const uint8_t reg2 = this->_readByte();
        this->_registers[reg1] = this->_stack[this->_registers[SP]--];
        this->_registers[reg2] = this->_stack[this->_registers[SP]--];
        printf("popped %d and %d\n", this->_registers[reg1], this->_registers[reg2]);
        break;
    }
    case DUP:
    {
        this->_registers[SP]++;
        this->_stack[this->_registers[SP]] = this->_stack[this->_registers[SP] - 1];
        break;
    }
    case STOR:
    {
        const int16_t addr = this->_readShort();
        const uint8_t reg = this->_readByte();
        *(int32_t *)&this->_data[addr] = this->_registers[reg];
        break;
    }
    case STORW:
    {
        const int16_t addr = this->_readShort();
        const uint8_t reg = this->_readByte();
        *(int16_t *)&this->_data[addr] = *(int16_t *)&this->_registers[reg];
        break;
    }
    case STORB:
    {
        const int16_t addr = this->_readShort();
        const uint8_t reg = this->_readByte();
        this->_data[addr] = *(uint8_t *)&this->_registers[reg];
        break;
    }
    case CSTORS:
    {
        const int16_t addr = this->_readShort();
        strcpy((char *)&this->_data[addr], (char *)&this->_program[++this->_registers[IP]]);
        this->_registers[IP] += strlen((char *)&this->_data[addr]);
        break;
    }
    case CSTORS_R:
    {
        const int16_t addr = this->_registers[this->_readByte()];
        strcpy((char *)&this->_data[addr], (char *)&this->_program[++this->_registers[IP]]);
        this->_registers[IP] += strlen((char *)&this->_data[addr]);
        break;
    }
    case LOAD:
    {
        const uint8_t reg = this->_readByte();
        const int16_t addr = this->_readShort();
        this->_registers[reg] = *(int32_t *)&this->_data[addr];
        break;
    }
    case LOADW:
    {
        const uint8_t reg = this->_readByte();
        const int16_t addr = this->_readShort();
        this->_registers[reg] = *(int16_t *)&this->_data[addr];
        break;
    }
    case LOADB:
    {
        const uint8_t reg = this->_readByte();
        const int16_t addr = this->_readShort();
        this->_registers[reg] = *(uint8_t *)&this->_data[addr];
        break;
    }
    case MEMCPY:
    {
        const int16_t bytes = this->_readShort();
        const int16_t dest = this->_readShort();
        const int16_t source = this->_readShort();
        memcpy((void *)&this->_data[dest], (void *)&this->_data[source], bytes);
        break;
    }
    case INCR:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg]++;
        break;
    }
    case INCRF:
    {
        const uint8_t reg = this->_readByte();
        (*((float *)&this->_registers[reg]))++;
        break;
    }
    case DECR:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg]--;
        break;
    }
    case DECRF:
    {
        const uint8_t reg = this->_readByte();
        (*((float *)&this->_registers[reg]))--;
        break;
    }
    case ADD:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] += this->_registers[this->_readByte()];
        break;
    }
    case ADDF:
    {
        const uint8_t reg = this->_readByte();
        *((float *)&this->_registers[reg]) += *((float *)&this->_registers[this->_readByte()]);
        break;
    }
    case SUB:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] -= this->_registers[this->_readByte()];
        break;
    }
    case SUBF:
    {
        const uint8_t reg = this->_readByte();
        *((float *)&this->_registers[reg]) -= *((float *)&this->_registers[this->_readByte()]);
        break;
    }
    case MULT:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] *= this->_registers[this->_readByte()];
        break;
    }
    case MULTF:
    {
        const uint8_t reg = this->_readByte();
        *((float *)&this->_registers[reg]) *= *((float *)&this->_registers[this->_readByte()]);
        break;
    }
    case DIV:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] /= this->_registers[this->_readByte()];
        break;
    }
    case DIVF:
    {
        const uint8_t reg = this->_readByte();
        *((float *)&this->_registers[reg]) /= *((float *)&this->_registers[this->_readByte()]);
        break;
    }
    case MOD:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] %= this->_registers[this->_readByte()];
        break;
    }
    case AND:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] &= this->_registers[this->_readByte()];
        break;
    }
    case OR:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] |= this->_registers[this->_readByte()];
        break;
    }
    case XOR:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] ^= this->_registers[this->_readByte()];
        break;
    }
    case NOT:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] = ~this->_registers[reg];
        break;
    }
    case I2F:
    {
        const uint8_t reg = this->_readByte();
        *((float *)&this->_registers[reg]) = (float)this->_registers[reg];
        break;
    }
    case F2I:
    {
        const uint8_t reg = this->_readByte();
        this->_registers[reg] = (int32_t) * ((float *)&this->_registers[reg]);
        break;
    }
    case JMP:
    {
        this->_registers[IP] = this->_readShort() - 1;
        break;
    }
    case JR:
    {
        this->_registers[IP] = this->_registers[this->_readByte()] - 1;
        break;
    }
    case JZ:
    {
        const uint8_t reg = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg] == 0)
            this->_registers[IP] = addr - 1;
        break;
    }
    case JNZ:
    {
        const uint8_t reg = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg] = !0)
            this->_registers[IP] = addr - 1;
        break;
    }
    case JGZ:
    {
        const uint8_t reg = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg] > 0)
            this->_registers[IP] = addr - 1;
        break;
    }
    case JLZ:
    {
        const uint8_t reg = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg] < 0)
            this->_registers[IP] = addr - 1;
        break;
    }
    case JE:
    {
        const uint8_t reg1 = this->_readByte();
        const uint8_t reg2 = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg1] == this->_registers[reg2])
            this->_registers[IP] = addr - 1;
        break;
    }
    case JNE:
    {
        const uint8_t reg1 = this->_readByte();
        const uint8_t reg2 = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg1] != this->_registers[reg2])
            this->_registers[IP] = addr - 1;
        break;
    }
    case JG:
    {
        const uint8_t reg1 = this->_readByte();
        const uint8_t reg2 = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg1] > this->_registers[reg2])
            this->_registers[IP] = addr - 1;
        break;
    }
    case JGE:
    {
        const uint8_t reg1 = this->_readByte();
        const uint8_t reg2 = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg1] >= this->_registers[reg2])
            this->_registers[IP] = addr - 1;
        break;
    }
    case JL:
    {
        const uint8_t reg1 = this->_readByte();
        const uint8_t reg2 = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg1] < this->_registers[reg2])
            this->_registers[IP] = addr - 1;
        break;
    }
    case JLE:
    {
        const uint8_t reg1 = this->_readByte();
        const uint8_t reg2 = this->_readByte();
        const int16_t addr = this->_readShort();

        if (this->_registers[reg1] <= this->_registers[reg2])
            this->_registers[IP] = addr - 1;
        break;
    }
    case PRINT:
    {
        const int32_t addr = this->_registers[this->_readByte()];
        char *curChar = (char *)&this->_data[addr];
        while (*curChar != '\0')
        {
            putchar(*curChar);
            curChar++;
        }
        break;
    }
    case PRINTI:
    {
        const uint8_t reg = this->_readByte();
        printf("%d", this->_registers[reg]);
        break;
    }
    case PRINTF:
    {
        const uint8_t reg = this->_readByte();
        printf("%f", *((float *)&this->_registers[reg]));
        break;
    }
    case PRINTLN:
    {
        putchar('\n');
        break;
    }
    case I2S:
    {
        const uint8_t reg = this->_readByte();
        const int16_t addr = this->_readShort();
        sprintf((char *)&this->_data[addr], "%d", this->_registers[reg]);
        break;
    }
    case S2I:
    {
        const int16_t addr = this->_readShort();
        const uint8_t reg = this->_readByte();
        sscanf((char *)&this->_data[addr], "%d", &this->_registers[reg]);
        break;
    }
    case A_DR:
    {
        const uint8_t reg = this->_readByte();
        const uint8_t pin = this->_readByte();

#ifdef ARDUINO
        this->_registers[reg] = digitalRead(pin);
#else
        this->_registers[reg] = 1;
#endif

        break;
    }
    case A_AR:
    {
        const uint8_t reg = this->_readByte();
        const uint8_t pin = this->_readByte();

#ifdef ARDUINO
        this->_registers[reg] = analogRead(pin);
#else
        this->_registers[reg] = 1024;
#endif

        break;
    }
    case A_DW:
    {
        const uint8_t pin = this->_readByte();
        const uint8_t state = this->_readByte();

#ifdef ARDUINO
        digitalWrite(pin, state);
#else
        printf("set pin %d to D%d\n", pin, state);
#endif

        break;
    }
    case A_AW:
    {
        const uint8_t pin = this->_readByte();
        const int16_t val = this->_readShort();

#ifdef ARDUINO
        analogWrite(pin, val);
#else
        printf("set pin %d to A%d\n", pin, val);
#endif

        break;
    }
    case A_AWR:
    {
        const uint8_t pin = this->_readByte();
        const uint8_t reg = this->_readByte();

#ifdef ARDUINO
        analogWrite(pin, this->_registers[reg]);
#else
        printf("set pin %d to A%d\n", pin, this->_registers[reg]);
#endif

        break;
    }
    case HALT:
    {
        this->_running = false;
        break;
    }
    }

    this->_registers[IP]++;
}

uint8_t VM::_readByte()
{
    return this->_program[++this->_registers[IP]];
}

int16_t VM::_readShort()
{
    const int16_t val = *((int16_t *)&this->_program[++this->_registers[IP]]);
    this->_registers[IP]++;
    return val;
}

int32_t VM::_readInt()
{
    const int32_t val = *((int32_t *)&this->_program[++this->_registers[IP]]);
    this->_registers[IP] += 3;
    return val;
}
