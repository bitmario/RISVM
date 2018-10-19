#include "vm.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s bin_file\n", argv[0]);
        return 1;
    }

    uint8_t *program;

    FILE *f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    long fileLen = ftell(f);
    rewind(f);

    program = (uint8_t *)malloc(fileLen);
    size_t s = fread(program, fileLen, 1, f);
    fclose(f);

    VM vm(program, fileLen);
    vm.run();

    return 0;
}
