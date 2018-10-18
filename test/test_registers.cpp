#include "test.h"

TEST_CASE("OP_LCONS")
{
    SECTION("Load zero")
    {
        uint8_t program[] = {
            OP_LCONS, R0, 0, 0, 0, 0,
            OP_HALT};
        VM vm(program);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("Load max")
    {
        uint8_t program[] = {
            OP_LCONS, R0, 0xFF, 0xFF, 0xFF, 0xFF,
            OP_HALT};
        VM vm(program);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == UINT32_MAX);
    }
}

TEST_CASE("OP_LCONSW")
{
    SECTION("Load zero")
    {
        uint8_t program[] = {
            OP_LCONSW, R0, 0, 0,
            OP_HALT};
        VM vm(program);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("Load max")
    {
        uint8_t program[] = {
            OP_LCONSW, R0, 0xFF, 0xFF,
            OP_HALT};
        VM vm(program);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == UINT16_MAX);
    }
}

TEST_CASE("OP_LCONSB")
{
    SECTION("Load zero")
    {
        uint8_t program[] = {
            OP_LCONSB, R0, 0,
            OP_HALT};
        VM vm(program);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("Load max")
    {
        uint8_t program[] = {
            OP_LCONSB, R0, 0xFF,
            OP_HALT};
        VM vm(program);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == UINT8_MAX);
    }
}

TEST_CASE("OP_MOV")
{
    uint8_t program[] = {
        OP_MOV, R0, R1,
        OP_HALT};
    VM vm(program);

    SECTION("Load zero")
    {
        vm.reset();
        vm.setRegister(R1, 0);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("Load 1122334455U")
    {
        vm.reset();
        vm.setRegister(R1, 1122334455U);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1122334455U);
    }

    SECTION("Load max")
    {
        vm.reset();
        vm.setRegister(R1, UINT32_MAX);
        vm.setRegister(R0, _U32_GARBAGE); // set garbage
        vm.run();
        REQUIRE(vm.getRegister(R0) == UINT32_MAX);
    }
}
