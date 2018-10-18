#include "test.h"

TEST_CASE("OP_INC")
{
    uint8_t program[] = {
        OP_INC, R0,
        OP_HALT};
    VM vm(program);

    SECTION("Zero")
    {
        vm.setRegister(R0, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("1123497651")
    {
        vm.reset();
        vm.setRegister(R0, 1123497651);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1123497652);
    }

    SECTION("UINT32_MAX")
    {
        vm.reset();
        vm.setRegister(R0, UINT32_MAX);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_FINC")
{
    uint8_t program[] = {
        OP_FINC, R0,
        OP_HALT};
    VM vm(program);

    SECTION("Zero")
    {
        float val = 0.0f;
        float expected = 1.0f;
        vm.setRegister(R0, *((uint32_t *)&val));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }

    SECTION("1123497.12")
    {
        float val = 1123497.12f;
        float expected = 1123498.12f;
        vm.setRegister(R0, *((uint32_t *)&val));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }

    SECTION("-5")
    {
        float val = -5.0f;
        float expected = -4.0f;
        vm.setRegister(R0, *((uint32_t *)&val));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }
}

TEST_CASE("OP_DEC")
{
    uint8_t program[] = {
        OP_DEC, R0,
        OP_HALT};
    VM vm(program);

    SECTION("Zero")
    {
        vm.setRegister(R0, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == UINT32_MAX);
    }

    SECTION("1123497651")
    {
        vm.reset();
        vm.setRegister(R0, 1123497651);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1123497650);
    }

    SECTION("UINT32_MAX")
    {
        vm.reset();
        vm.setRegister(R0, UINT32_MAX);
        vm.run();
        REQUIRE(vm.getRegister(R0) == (UINT32_MAX - 1));
    }
}

TEST_CASE("OP_FDEC")
{
    uint8_t program[] = {
        OP_FDEC, R0,
        OP_HALT};
    VM vm(program);

    SECTION("Zero")
    {
        float val = 0.0f;
        float expected = -1.0f;
        vm.setRegister(R0, *((uint32_t *)&val));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }

    SECTION("1123497.12")
    {
        float val = 1123497.12f;
        float expected = 1123496.12f;
        vm.setRegister(R0, *((uint32_t *)&val));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }

    SECTION("-5")
    {
        float val = -5.0f;
        float expected = -6.0f;
        vm.setRegister(R0, *((uint32_t *)&val));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }
}

TEST_CASE("OP_ADD")
{
    uint8_t program[] = {
        OP_ADD, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("0 + 0")
    {
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1123497651 + 987513")
    {
        vm.reset();
        vm.setRegister(R1, 1123497651);
        vm.setRegister(R2, 987513);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1124485164);
    }

    SECTION("UINT32_MAX + 1")
    {
        vm.reset();
        vm.setRegister(R1, UINT32_MAX);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_FADD")
{
    uint8_t program[] = {
        OP_FADD, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("0 + 0")
    {
        float val = 0.0f;
        vm.setRegister(R1, *((uint32_t *)&val));
        vm.setRegister(R2, *((uint32_t *)&val));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), val));
    }

    SECTION("357.34 + 847.21")
    {
        float val1 = 357.34f;
        float val2 = 847.21f;
        float expected = 1204.55f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }

    SECTION("-3 + -5")
    {
        float val1 = -3.0f;
        float val2 = -5.0f;
        float expected = -8.0f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }
}

TEST_CASE("OP_SUB")
{
    uint8_t program[] = {
        OP_SUB, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("0 - 0")
    {
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1123497651 - 987513")
    {
        vm.reset();
        vm.setRegister(R1, 1123497651);
        vm.setRegister(R2, 987513);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1122510138);
    }

    SECTION("0 - 1")
    {
        vm.reset();
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == UINT32_MAX);
    }
}

TEST_CASE("OP_FSUB")
{
    uint8_t program[] = {
        OP_FSUB, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("0 - 0")
    {
        float val = 0.0f;
        vm.setRegister(R1, *((uint32_t *)&val));
        vm.setRegister(R2, *((uint32_t *)&val));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), val));
    }

    SECTION("357.34 - 847.21")
    {
        float val1 = 357.34f;
        float val2 = 847.21f;
        float expected = -489.87f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }

    SECTION("-3 - -5")
    {
        float val1 = -3.0f;
        float val2 = -5.0f;
        float expected = 2.0f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }
}

TEST_CASE("OP_MUL")
{
    uint8_t program[] = {
        OP_MUL, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("1 * 0")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("2 * 2")
    {
        vm.reset();
        vm.setRegister(R1, 2);
        vm.setRegister(R2, 2);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 4);
    }

    SECTION("347 * 987513")
    {
        vm.reset();
        vm.setRegister(R1, 347);
        vm.setRegister(R2, 987513);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 342667011);
    }
}

TEST_CASE("OP_IMUL")
{
    uint8_t program[] = {
        OP_IMUL, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("1 * 0")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("2 * 2")
    {
        vm.reset();
        vm.setRegister(R1, 2);
        vm.setRegister(R2, 2);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 4);
    }

    SECTION("-3 * -5")
    {
        int32_t val1 = -3;
        int32_t val2 = -5;
        int32_t expected = 15;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }
}

TEST_CASE("OP_FMUL")
{
    uint8_t program[] = {
        OP_FMUL, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("1 * 0")
    {
        float val1 = 1.0f;
        float val2 = 0.0f;
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&val2));
    }

    SECTION("357.34 * 847.21")
    {
        float val1 = 357.34f;
        float val2 = 847.21f;
        float expected = 302742.0214f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }

    SECTION("-3 * -5")
    {
        float val1 = -3.0f;
        float val2 = -5.0f;
        float expected = 15.0f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }
}

TEST_CASE("OP_DIV")
{
    uint8_t program[] = {
        OP_DIV, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    // SECTION("1 / 0")
    // {
    // TODO: add code to test handling of division by zero
    // }

    SECTION("2 / 2")
    {
        vm.reset();
        vm.setRegister(R1, 2);
        vm.setRegister(R2, 2);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("987513 / 347")
    {
        vm.reset();
        vm.setRegister(R1, 987513);
        vm.setRegister(R2, 347);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 2845);
    }
}

TEST_CASE("OP_IDIV")
{
    uint8_t program[] = {
        OP_IDIV, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    // SECTION("1 / 0")
    // {
    // TODO: add code to test handling of division by zero
    // }

    SECTION("2 / 2")
    {
        vm.reset();
        vm.setRegister(R1, 2);
        vm.setRegister(R2, 2);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("-50 / -3")
    {
        int32_t val1 = -50;
        int32_t val2 = -3;
        int32_t expected = 16;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }
}

TEST_CASE("OP_FDIV")
{
    uint8_t program[] = {
        OP_FDIV, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    // SECTION("1 / 0")
    // {
    // TODO: add code to test handling of division by zero
    // }

    SECTION("847.21 / 357.34")
    {
        float val1 = 847.21f;
        float val2 = 357.34f;
        float expected = 2.370879275f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }

    SECTION("-50 / -3")
    {
        float val1 = -50.0f;
        float val2 = -3.0f;
        float expected = 16.666666667f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float *)&actual), expected));
    }
}

TEST_CASE("OP_SHL")
{
    uint8_t program[] = {
        OP_SHL, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("1 << 0")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("0xFF00 << 1")
    {
        vm.reset();
        vm.setRegister(R1, 0xFF00);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0x1FE00);
    }

    SECTION("0xA310 << 16")
    {
        vm.reset();
        vm.setRegister(R1, 0xA310);
        vm.setRegister(R2, 16);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0xA3100000);
    }
}

TEST_CASE("OP_SHR")
{
    uint8_t program[] = {
        OP_SHR, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("1 >> 0")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("0xFF00 >> 1")
    {
        vm.reset();
        vm.setRegister(R1, 0xFF00);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0x7F80);
    }

    SECTION("0xA3100000 >> 16")
    {
        vm.reset();
        vm.setRegister(R1, 0xA3100000);
        vm.setRegister(R2, 16);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0xA310);
    }
}

TEST_CASE("OP_ISHR")
{
    uint8_t program[] = {
        OP_ISHR, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("1 >> 0")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("-100 >> 1")
    {
        int32_t val1 = -100;
        int32_t expected = -50;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }

    SECTION("0xA3100000 >> 16")
    {
        int32_t val1 = 100;
        int32_t expected = 50;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }
}

TEST_CASE("OP_MOD")
{
    uint8_t program[] = {
        OP_MOD, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    // SECTION("1 % 0")
    // {
    // TODO: add code to test handling of division by zero
    // }

    SECTION("2 mod 2")
    {
        vm.reset();
        vm.setRegister(R1, 2);
        vm.setRegister(R2, 2);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("987513 mod 347")
    {
        vm.reset();
        vm.setRegister(R1, 987513);
        vm.setRegister(R2, 347);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 298);
    }
}

TEST_CASE("OP_IMOD")
{
    uint8_t program[] = {
        OP_IMOD, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    // SECTION("1 / 0")
    // {
    // TODO: add code to test handling of division by zero
    // }

    SECTION("2 mod 2")
    {
        vm.reset();
        vm.setRegister(R1, 2);
        vm.setRegister(R2, 2);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("-50 mod -3")
    {
        int32_t val1 = -50;
        int32_t val2 = -3;
        int32_t expected = -2;
        vm.reset();
        vm.setRegister(R1, *((uint32_t *)&val1));
        vm.setRegister(R2, *((uint32_t *)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t *)&expected));
    }
}

TEST_CASE("OP_AND")
{
    uint8_t program[] = {
        OP_AND, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("0 & 0")
    {
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1 & 0")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1 & 1")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("0xF1F1F1F1 & 0xEAD1")
    {
        vm.setRegister(R1, 0xF1F1F1F1);
        vm.setRegister(R2, 0xEAD1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0xE0D1);
    }
}

TEST_CASE("OP_OR")
{
    uint8_t program[] = {
        OP_OR, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("0 | 0")
    {
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1 | 0")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("1 | 1")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("0xF1F1F1F1 | 0xEAD1")
    {
        vm.setRegister(R1, 0xF1F1F1F1);
        vm.setRegister(R2, 0xEAD1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0xF1F1FBF1);
    }
}

TEST_CASE("OP_XOR")
{
    uint8_t program[] = {
        OP_XOR, R0, R1, R2,
        OP_HALT};
    VM vm(program);

    SECTION("0 ^ 0")
    {
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1 ^ 0")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1);
    }

    SECTION("1 ^ 1")
    {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("0xF1F1F1F1 ^ 0xEAD1")
    {
        vm.setRegister(R1, 0xF1F1F1F1);
        vm.setRegister(R2, 0xEAD1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0xF1F11B20);
    }
}

TEST_CASE("OP_NOT")
{
    uint8_t program[] = {
        OP_NOT, R0, R1,
        OP_HALT};
    VM vm(program);

    SECTION("!0")
    {
        vm.setRegister(R1, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0xFFFFFFFF);
    }

    SECTION("!1")
    {
        vm.setRegister(R1, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0xFFFFFFFE);
    }

    SECTION("!0xF1F1F1F1")
    {
        vm.setRegister(R1, 0xF1F1F1F1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0xE0E0E0E);
    }
}
