#include "test.h"

TEST_CASE("OP_F2I")
{
    uint8_t program[] = {
        OP_F2I, R0, R1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("1345.937")
    {
        float val = 1345.937f;
        int32_t expected = 1345;
        vm.setRegister(R1, *((uint32_t *)&val));
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);

        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }

    SECTION("-781345.719")
    {
        float val = -781345.719;
        int32_t expected = -781345;
        vm.setRegister(R1, *((uint32_t *)&val));
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        
        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }
}

TEST_CASE("OP_I2F")
{
    uint8_t program[] = {
        OP_I2F, R0, R1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("1345")
    {
        int32_t val = 1345;
        float expected = 1345.0f;
        vm.setRegister(R1, *((uint32_t *)&val));
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);

        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }

    SECTION("-781345")
    {
        int32_t val = -781345;
        float expected = -781345.0f;
        vm.setRegister(R1, *((uint32_t *)&val));
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);

        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }
}
