#include "test.h"

TEST_CASE("OP_STOR")
{
    uint8_t program[] = {
        OP_STOR, 5, 0, R0,
        OP_HALT,
        0, 0, 0, 0, 0xFF};
    VM vm(program, sizeof(program));
    uint32_t actual = 0;

    SECTION("Store value")
    {
        vm.setRegister(R0, _U32_GARBAGE);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[5], 4);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(memory[9] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[5], 4);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(memory[9] == 0xFF);
    }
}

TEST_CASE("OP_STOR_P")
{
    uint8_t program[] = {
        OP_STOR_P, R1, R0,
        OP_HALT,
        0, 0, 0, 0, 0xFF};
    VM vm(program, sizeof(program));
    uint32_t actual = 0;

    SECTION("Store value")
    {
        vm.setRegister(R1, 4);
        vm.setRegister(R0, _U32_GARBAGE);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[4], 4);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(memory[8] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R1, 4);
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[4], 4);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(memory[8] == 0xFF);
    }
}

TEST_CASE("OP_STORW")
{
    uint8_t program[] = {
        OP_STORW, 5, 0, R0,
        OP_HALT,
        0, 0, 0xFF};
    VM vm(program, sizeof(program));
    uint16_t actual = 0;

    SECTION("Store value")
    {
        vm.setRegister(R0, _U16_GARBAGE);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[5], 2);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == _U16_GARBAGE);
        REQUIRE(memory[7] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[5], 2);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(memory[7] == 0xFF);
    }
}

TEST_CASE("OP_STORW_P")
{
    uint8_t program[] = {
        OP_STORW_P, R1, R0,
        OP_HALT,
        0, 0, 0xFF};
    VM vm(program, sizeof(program));
    uint16_t actual = 0;

    SECTION("Store value")
    {
        vm.setRegister(R1, 4);
        vm.setRegister(R0, _U16_GARBAGE);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[4], 2);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(actual == _U16_GARBAGE);
        REQUIRE(memory[6] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R1, 4);
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[4], 2);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(memory[6] == 0xFF);
    }
}

TEST_CASE("OP_STORB")
{
    uint8_t program[] = {
        OP_STORB, 5, 0, R0,
        OP_HALT,
        0, 0xFF};
    VM vm(program, sizeof(program));

    SECTION("Store value")
    {
        vm.setRegister(R0, _U8_GARBAGE);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(memory[5] == _U8_GARBAGE);
        REQUIRE(memory[6] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(memory[5] == 0);
        REQUIRE(memory[6] == 0xFF);
    }
}

TEST_CASE("OP_STORB_P")
{
    uint8_t program[] = {
        OP_STORB_P, R1, R0,
        OP_HALT,
        0, 0xFF};
    VM vm(program, sizeof(program));

    SECTION("Store value")
    {
        vm.setRegister(R1, 4);
        vm.setRegister(R0, _U8_GARBAGE);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(memory[4] == _U8_GARBAGE);
        REQUIRE(memory[5] == 0xFF);
    }

    SECTION("Store zero")
    {
        vm.reset();
        vm.setRegister(R1, 4);
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(memory[4] == 0);
        REQUIRE(memory[5] == 0xFF);
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
    VM vm(program, sizeof(program));
    uint32_t actual = 0;

    SECTION("Load value")
    {
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[5], 4);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U32_GARBAGE);
        REQUIRE(memory[9] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        uint8_t *memory = vm.memory();

        memset(&memory[5], 0, 4);
        vm.setRegister(R0, _U32_GARBAGE);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        memcpy(&actual, &memory[5], 4);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(memory[9] == 0xFF);
    }
}

TEST_CASE("OP_LOAD_P")
{
    uint8_t program[] = {
        OP_LOAD_P, R0, R1,
        OP_HALT,
        _NTH_BYTE(_U32_GARBAGE, 0), _NTH_BYTE(_U32_GARBAGE, 1),
        _NTH_BYTE(_U32_GARBAGE, 2), _NTH_BYTE(_U32_GARBAGE, 3),
        0xFF};
    VM vm(program, sizeof(program));
    uint32_t actual = 0;

    SECTION("Load value")
    {
        vm.setRegister(R1, 4);
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[4], 4);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U32_GARBAGE);
        REQUIRE(memory[8] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        vm.setRegister(R1, 4);
        vm.setRegister(R0, _U32_GARBAGE);
        
        uint8_t *memory = vm.memory();
        memset(&memory[4], 0, 4);

        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        memcpy(&actual, &memory[4], 4);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(memory[8] == 0xFF);
    }
}

TEST_CASE("OP_LOADW")
{
    uint8_t program[] = {
        OP_LOADW, R0, 5, 0,
        OP_HALT,
        _NTH_BYTE(_U16_GARBAGE, 0), _NTH_BYTE(_U16_GARBAGE, 1),
        0xFF};
    VM vm(program, sizeof(program));
    uint16_t actual = 0;

    SECTION("Load value")
    {
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[5], 2);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == _U16_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U16_GARBAGE);
        REQUIRE(memory[7] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        uint8_t *memory = vm.memory();

        memset(&memory[5], 0, 2);
        vm.setRegister(R0, _U32_GARBAGE);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        memcpy(&actual, &memory[5], 2);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(memory[7] == 0xFF);
    }
}

TEST_CASE("OP_LOADW_P")
{
    uint8_t program[] = {
        OP_LOADW_P, R0, R1,
        OP_HALT,
        _NTH_BYTE(_U16_GARBAGE, 0), _NTH_BYTE(_U16_GARBAGE, 1),
        0xFF};
    VM vm(program, sizeof(program));
    uint16_t actual = 0;

    SECTION("Load value")
    {
        vm.setRegister(R1, 4);
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[4], 2);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(actual == _U16_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U16_GARBAGE);
        REQUIRE(memory[6] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        vm.setRegister(R1, 4);
        vm.setRegister(R0, _U32_GARBAGE);
        
        uint8_t *memory = vm.memory();
        memset(&memory[4], 0, 2);

        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        memcpy(&actual, &memory[4], 2);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(actual == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(memory[6] == 0xFF);
    }
}

TEST_CASE("OP_LOADB")
{
    uint8_t program[] = {
        OP_LOADB, R0, 5, 0,
        OP_HALT,
        _U8_GARBAGE,
        0xFF};
    VM vm(program, sizeof(program));

    SECTION("Load value")
    {
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(memory[5] == _U8_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U8_GARBAGE);
        REQUIRE(memory[6] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        uint8_t *memory = vm.memory();

        vm.setRegister(R0, _U32_GARBAGE);
        memory[5] = 0;
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(memory[5] == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(memory[6] == 0xFF);
    }
}

TEST_CASE("OP_LOADB_P")
{
    uint8_t program[] = {
        OP_LOADB_P, R0, R1,
        OP_HALT,
        _U8_GARBAGE,
        0xFF};
    VM vm(program, sizeof(program));

    SECTION("Load value")
    {
        vm.setRegister(R1, 4);
        vm.setRegister(R0, 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(memory[4] == _U8_GARBAGE);
        REQUIRE(vm.getRegister(R0) == _U8_GARBAGE);
        REQUIRE(memory[5] == 0xFF);
    }

    SECTION("Load zero")
    {
        vm.reset();
        vm.setRegister(R1, 4);
        vm.setRegister(R0, _U32_GARBAGE);
        
        uint8_t *memory = vm.memory();
        memory[4] = 0;

        REQUIRE(vm.run() == ExecResult::VM_FINISHED);

        REQUIRE(memory[3] == OP_HALT);
        REQUIRE(memory[4] == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(memory[5] == 0xFF);
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
    VM vm(program, sizeof(program));
    uint32_t actual = 0;
    uint32_t source = 0;

    SECTION("Copy 4 bytes")
    {
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[8], 4);
        memcpy(&source, &memory[12], 4);

        REQUIRE(memory[7] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(source == _U32_GARBAGE);
        REQUIRE(memory[16] == 0xFF);
    }
}

TEST_CASE("OP_MEMCPY_P")
{
    uint8_t program[] = {
        OP_MEMCPY_P, R0, R1, R2,
        OP_HALT,
        0, 0, 0, 0,
        _NTH_BYTE(_U32_GARBAGE, 0), _NTH_BYTE(_U32_GARBAGE, 1),
        _NTH_BYTE(_U32_GARBAGE, 2), _NTH_BYTE(_U32_GARBAGE, 3),
        0xFF};
    VM vm(program, sizeof(program));
    uint32_t actual = 0;
    uint32_t source = 0;

    SECTION("Copy 4 bytes")
    {
        vm.setRegister(R0, 5);
        vm.setRegister(R1, 9);
        vm.setRegister(R2, 4);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);

        uint8_t *memory = vm.memory();
        memcpy(&actual, &memory[5], 4);
        memcpy(&source, &memory[9], 4);

        REQUIRE(memory[4] == OP_HALT);
        REQUIRE(actual == _U32_GARBAGE);
        REQUIRE(source == _U32_GARBAGE);
        REQUIRE(memory[13] == 0xFF);
    }
}
