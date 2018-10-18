#include "test.h"

TEST_CASE("OP_STOR")
{
    uint8_t program[] = {
        OP_STOR, 5, 0, R0,
        OP_HALT,
        0, 0, 0, 0, 0xFF};
    VM vm(program);
    uint32_t actual = 0;

    SECTION("Store value")
    {
        vm.setRegister(R0, _U32_GARBAGE);
        vm.run();
        memcpy(&actual, &program[5], 4);

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(program[9] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R0, 0);
        vm.run();
        memcpy(&actual, &program[5], 4);

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(program[9] == 0xFF);
    }
}

TEST_CASE("OP_STORW")
{
    uint8_t program[] = {
        OP_STORW, 5, 0, R0,
        OP_HALT,
        0, 0, 0xFF};
    VM vm(program);
    uint16_t actual = 0;

    SECTION("Store value")
    {
        vm.setRegister(R0, _U16_GARBAGE);
        vm.run();
        memcpy(&actual, &program[5], 2);

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(actual == _U16_GARBAGE);
        REQUIRE(program[7] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R0, 0);
        vm.run();
        memcpy(&actual, &program[5], 2);

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(program[7] == 0xFF);
    }
}

TEST_CASE("OP_STORB")
{
    uint8_t program[] = {
        OP_STORB, 5, 0, R0,
        OP_HALT,
        0, 0xFF};
    VM vm(program);

    SECTION("Store value")
    {
        vm.setRegister(R0, _U8_GARBAGE);
        vm.run();

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(program[5] == _U8_GARBAGE);
        REQUIRE(program[6] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R0, 0);
        vm.run();

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(program[5] == 0);
        REQUIRE(program[6] == 0xFF);
    }
}

TEST_CASE("OP_LOAD")
{
    uint8_t program[] = {
        OP_LOAD, R0, 5, 0,
        OP_HALT,
        _NTH_BYTE(_U32_GARBAGE, 0), _NTH_BYTE(_U32_GARBAGE, 1),
        _NTH_BYTE(_U32_GARBAGE, 2), _NTH_BYTE(_U32_GARBAGE, 3),
        0xFF};
    VM vm(program);
    uint32_t actual = 0;

    SECTION("Load value")
    {
        vm.run();
        memcpy(&actual, &program[5], 4);

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U32_GARBAGE);
        REQUIRE(program[9] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        memset(&program[5], 0, 4);
        vm.setRegister(R0, _U32_GARBAGE);
        vm.run();
        memcpy(&actual, &program[5], 4);

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(program[9] == 0xFF);
    }
}

TEST_CASE("OP_LOADW")
{
    uint8_t program[] = {
        OP_LOADW, R0, 5, 0,
        OP_HALT,
        _NTH_BYTE(_U16_GARBAGE, 0), _NTH_BYTE(_U16_GARBAGE, 1),
        0xFF};
    VM vm(program);
    uint16_t actual = 0;

    SECTION("Load value")
    {
        vm.run();
        memcpy(&actual, &program[5], 2);

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(actual == _U16_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U16_GARBAGE);
        REQUIRE(program[7] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        memset(&program[5], 0, 2);
        vm.setRegister(R0, _U32_GARBAGE);
        vm.run();
        memcpy(&actual, &program[5], 2);

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(program[7] == 0xFF);
    }
}

TEST_CASE("OP_LOADB")
{
    uint8_t program[] = {
        OP_LOADB, R0, 5, 0,
        OP_HALT,
        _U8_GARBAGE,
        0xFF};
    VM vm(program);

    SECTION("Load value")
    {
        vm.run();

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(program[5] == _U8_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U8_GARBAGE);
        REQUIRE(program[6] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        vm.setRegister(R0, _U32_GARBAGE);
        program[5] = 0;
        vm.run();

        REQUIRE(program[4] == OP_HALT);
        REQUIRE(program[5] == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(program[6] == 0xFF);
    }
}

TEST_CASE("OP_MEMCPY")
{
    uint8_t program[] = {
        OP_MEMCPY, 8, 0, 12, 0, 4, 0, // copy 4 bytes from 0xC to 0x8
        OP_HALT,
        0, 0, 0, 0,
        _NTH_BYTE(_U32_GARBAGE, 0), _NTH_BYTE(_U32_GARBAGE, 1),
        _NTH_BYTE(_U32_GARBAGE, 2), _NTH_BYTE(_U32_GARBAGE, 3),
        0xFF};
    VM vm(program);
    uint32_t actual = 0;
    uint32_t source = 0;

    SECTION("Copy 4 bytes")
    {
        vm.run();
        memcpy(&actual, &program[8], 4);
        memcpy(&source, &program[12], 4);

        REQUIRE(program[7] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(source == _U32_GARBAGE);
        REQUIRE(program[16] == 0xFF);
    }
}
