#include "vm.h"
#include "programs.h"

const uint8_t test1[] = {
    0x12, 0x00, 0x7F, 0x30, 0x01, 0x00, 0xD1, 0x01, 0xFF,
};

int main()
{
    //VM vm(program3, program3Data, sizeof(program3Data));
    //VM vm(program, NULL, 0, 256);
    //VM vm(benchmark, NULL, 0, 0);
    VM vm(test1, NULL, 0, 0);
    vm.run();

    return 0;
}
