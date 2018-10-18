#include "test.h"

TEST_CASE("OP_PUSH")
{
    uint8_t program[] = {
        OP_PUSH, R0,
        OP_HALT};
    VM vm(program);

    SECTION("Zero")
    {
        vm.setRegister(R0, 0);
        vm.run();
        REQUIRE(vm.stackCount() == 1);
        REQUIRE(vm.stackPop() == 0);
    }

    SECTION("1123497651")
    {
        vm.reset();
        vm.setRegister(R0, 1123497651);
        vm.run();
        REQUIRE(vm.stackCount() == 1);
        REQUIRE(vm.stackPop() == 1123497651);
    }

    SECTION("UINT32_MAX")
    {
        vm.reset();
        vm.setRegister(R0, UINT32_MAX);
        vm.run();
        REQUIRE(vm.stackCount() == 1);
        REQUIRE(vm.stackPop() == UINT32_MAX);
    }
}

TEST_CASE("OP_POP")
{
    uint8_t program[] = {
        OP_POP, R0,
        OP_HALT};
    VM vm(program);

    SECTION("Zero")
    {
        vm.stackPush(0);
        vm.run();
        REQUIRE(vm.stackCount() == 0);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1123497651")
    {
        vm.reset();
        vm.stackPush(1123497651);
        vm.run();
        REQUIRE(vm.stackCount() == 0);
        REQUIRE(vm.getRegister(R0) == 1123497651);
    }

    SECTION("UINT32_MAX")
    {
        vm.reset();
        vm.stackPush(UINT32_MAX);
        vm.run();
        REQUIRE(vm.stackCount() == 0);
        REQUIRE(vm.getRegister(R0) == UINT32_MAX);
    }
}

TEST_CASE("OP_POP2")
{
    uint8_t program[] = {
        OP_POP2, R0, R1,
        OP_HALT};
    VM vm(program);

    SECTION("Zero")
    {
        vm.stackPush(0);
        vm.stackPush(0);
        vm.run();
        REQUIRE(vm.stackCount() == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.getRegister(R1) == 0);
    }

    SECTION("1123497651 / 123")
    {
        vm.reset();
        vm.stackPush(1123497651);
        vm.stackPush(123);
        vm.run();
        REQUIRE(vm.stackCount() == 0);
        REQUIRE(vm.getRegister(R0) == 123);
        REQUIRE(vm.getRegister(R1) == 1123497651);
    }

    SECTION("UINT32_MAX")
    {
        vm.reset();
        vm.stackPush(UINT32_MAX);
        vm.stackPush(UINT32_MAX);
        vm.run();
        REQUIRE(vm.stackCount() == 0);
        REQUIRE(vm.getRegister(R0) == UINT32_MAX);
        REQUIRE(vm.getRegister(R1) == UINT32_MAX);
    }
}

TEST_CASE("OP_DUP")
{
    uint8_t program[] = {
        OP_DUP,
        OP_HALT};
    VM vm(program);

    SECTION("Zero")
    {
        vm.stackPush(0);
        vm.run();
        REQUIRE(vm.stackCount() == 2);
        REQUIRE(vm.stackPop() == 0);
        REQUIRE(vm.stackPop() == 0);
    }

    SECTION("1123497651 ")
    {
        vm.reset();
        vm.stackPush(1123497651);
        vm.run();
        REQUIRE(vm.stackCount() == 2);
        REQUIRE(vm.stackPop() == 1123497651);
        REQUIRE(vm.stackPop() == 1123497651);
    }

    SECTION("UINT32_MAX")
    {
        vm.reset();
        vm.stackPush(UINT32_MAX);
        vm.run();
        REQUIRE(vm.stackCount() == 2);
        REQUIRE(vm.stackPop() == UINT32_MAX);
        REQUIRE(vm.stackPop() == UINT32_MAX);
    }
}
