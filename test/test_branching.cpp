#include "test.h"

TEST_CASE("OP_JMP")
{
    SECTION("Jump and set 1")
    {
        uint8_t program[] = {
            OP_JMP, 4, 0,
            OP_HALT,
            OP_LCONSB, R0, 1,
            OP_HALT};
        VM vm(program, sizeof(program));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }
}

TEST_CASE("OP_JR")
{
    SECTION("Jump and set 1")
    {
        uint8_t program[] = {
            OP_JR, R1,
            OP_HALT,
            OP_LCONSB, R0, 1,
            OP_HALT};
        VM vm(program, sizeof(program));
        vm.setRegister(R1, 3);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }
}

TEST_CASE("OP_JZ")
{
    uint8_t program[] = {
        OP_JZ, R1, 5, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("True")
    {
        REQUIRE(vm.getRegister(R1) == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("False")
    {
        vm.reset();
        vm.setRegister(R1, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_JNZ")
{
    uint8_t program[] = {
        OP_JNZ, R1, 5, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("True")
    {
        vm.setRegister(R1, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("False")
    {
        vm.reset();
        REQUIRE(vm.getRegister(R1) == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_JE")
{
    uint8_t program[] = {
        OP_JE, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("True")
    {
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("False")
    {
        vm.reset();
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R1) == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_JNE")
{
    uint8_t program[] = {
        OP_JNE, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("False")
    {
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("True")
    {
        vm.reset();
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R1) == 0);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }
}

TEST_CASE("OP_JA")
{
    uint8_t program[] = {
        OP_JA, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("True")
    {
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 122);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("False - equal")
    {
        vm.reset();
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("False - below")
    {
        vm.reset();
        vm.setRegister(R1, 122);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_JG")
{
    uint8_t program[] = {
        OP_JG, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("True - negative")
    {
        int32_t val1 = -122;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("True - neg/pos")
    {
        vm.reset();
        int32_t val1 = 1;
        int32_t val2 = -122;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("False - equal")
    {
        vm.reset();
        int32_t val1 = -123;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("False - below negative")
    {
        vm.reset();
        int32_t val1 = -124;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("False - below neg/pos")
    {
        vm.reset();
        int32_t val1 = -124;
        int32_t val2 = 1;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_JAE")
{
    uint8_t program[] = {
        OP_JAE, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("True - above")
    {
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 122);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("True - equal")
    {
        vm.reset();
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("False - below")
    {
        vm.reset();
        vm.setRegister(R1, 122);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_JGE")
{
    uint8_t program[] = {
        OP_JGE, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("True - greater negative")
    {
        int32_t val1 = -122;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("True - greater neg/pos")
    {
        vm.reset();
        int32_t val1 = 1;
        int32_t val2 = -122;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("True - equal")
    {
        vm.reset();
        int32_t val1 = -123;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("False - below negative")
    {
        vm.reset();
        int32_t val1 = -124;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("False - below neg/pos")
    {
        vm.reset();
        int32_t val1 = -124;
        int32_t val2 = 1;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_JB")
{
    uint8_t program[] = {
        OP_JB, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("False - above")
    {
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 122);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("False - equal")
    {
        vm.reset();
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("True - below")
    {
        vm.reset();
        vm.setRegister(R1, 122);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }
}

TEST_CASE("OP_JL")
{
    uint8_t program[] = {
        OP_JL, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("False - greater negative")
    {
        int32_t val1 = -122;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("False - greater neg/pos")
    {
        vm.reset();
        int32_t val1 = 1;
        int32_t val2 = -122;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("False - equal")
    {
        vm.reset();
        int32_t val1 = -123;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("True - below negative")
    {
        vm.reset();
        int32_t val1 = -124;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("True - below neg/pos")
    {
        vm.reset();
        int32_t val1 = -124;
        int32_t val2 = 1;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }
}

TEST_CASE("OP_JBE")
{
    uint8_t program[] = {
        OP_JBE, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("False - above")
    {
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 122);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("True - equal")
    {
        vm.reset();
        vm.setRegister(R1, 123);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("True - below")
    {
        vm.reset();
        vm.setRegister(R1, 122);
        vm.setRegister(R2, 123);
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }
}

TEST_CASE("OP_JLE")
{
    uint8_t program[] = {
        OP_JLE, R1, R2, 6, 0,
        OP_HALT,
        OP_LCONSB, R0, 1,
        OP_HALT};
    VM vm(program, sizeof(program));

    SECTION("False - greater negative")
    {
        int32_t val1 = -122;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("False - greater neg/pos")
    {
        vm.reset();
        int32_t val1 = 1;
        int32_t val2 = -122;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("True - equal")
    {
        vm.reset();
        int32_t val1 = -123;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("True - below negative")
    {
        vm.reset();
        int32_t val1 = -124;
        int32_t val2 = -123;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("True - below neg/pos")
    {
        vm.reset();
        int32_t val1 = -124;
        int32_t val2 = 1;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        REQUIRE(vm.getRegister(R0) == 0);
        REQUIRE(vm.run() == ExecResult::VM_FINISHED);
        REQUIRE(vm.getRegister(R0) == 1);
    }
}
