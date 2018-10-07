#include "vm.h"
#include "programs.h"

int main()
{
    //VM vm(program3, program3Data, sizeof(program3Data));
    //VM vm(program, NULL, 0, 256);
    VM vm(benchmark, NULL, 0, 0);
    vm.run();

    return 0;
}
