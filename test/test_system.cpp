#include "test.h"

uint8_t intCode;
bool intContinue;

bool handleInterrupt(uint8_t code)
{
    intCode = code;
    return intContinue;
}

TEST_CASE("OP_INT")
{
    SECTION("Code is correct")
    {
        uint8_t program[] = {
            OP_INT, 3,
            OP_HALT};
        VM vm(program);
        vm.onInterrupt(handleInterrupt);
        intContinue = true;
        intCode = 0;

        vm.run();
        REQUIRE(intCode == 3);

        program[1] = 253;
        vm.reset();
        vm.run();
        REQUIRE(intCode == 253);
    }

    SECTION("Can stop exec")
    {
        uint8_t program[] = {
            OP_INT, 3,
            OP_LCONSB, R0, 123,
            OP_HALT};
        VM vm(program);
        vm.onInterrupt(handleInterrupt);
        intContinue = true;
        intCode = 0;

        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(intCode == 3);
        REQUIRE(vm.getRegister(R0) == 123);
        REQUIRE(vm.getRegister(IP) == 5);

        intContinue = false;
        intCode = 0;
        vm.reset();
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(intCode == 3);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.getRegister(IP) == 1);
    }

    SECTION("Unhandled interrupt causes error")
    {
        uint8_t program[] = {
            OP_INT, 3,
            OP_LCONSB, R0, 123,
            OP_HALT};
        VM vm(program);
        REQUIRE(vm.run() == ExecResult::VM_ERR_UNHANDLED_INTERRUPT);
        REQUIRE(vm.getRegister(IP) == 1);
    }
}

TEST_CASE("OP_HALT")
{
    uint8_t program[] = {
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program);
    vm.setRegister(R0, 123);
    vm.run();
    REQUIRE(vm.getRegister(R0) == 123);
    REQUIRE(vm.getRegister(IP) == 0);
}

TEST_CASE("OP_NOP")
{
    uint8_t program[] = {
        OP_NOP,
        OP_HALT};
    VM vm(program);
    vm.run();

    SECTION("Registers unchanged")
    {
        for (uint8_t i = R0; i <= T9; i++)
            REQUIRE(vm.getRegister((Register)i) == 0);
    }

    SECTION("Correct IP")
    {
        REQUIRE(vm.getRegister(IP) == 1);
    }
}
