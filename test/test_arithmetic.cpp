#include <math.h>
#include "lib/catch.hpp"
#include "../src/vm.h"

#define _ALMOST_EQUAL(x, y) fabs(x - y) < 0.001

TEST_CASE("OP_ADD") {
    uint8_t program[] = {
        OP_ADD, R0, R1, R2,
        OP_HALT
    };
    VM vm(program);

    SECTION("0 + 0") {
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1123497651 + 987513") {
        vm.reset();
        vm.setRegister(R1, 1123497651);
        vm.setRegister(R2, 987513);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1124485164);
    }

    SECTION("UINT32_MAX + 1") {
        vm.reset();
        vm.setRegister(R1, UINT32_MAX);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }
}

TEST_CASE("OP_FADD") {
    uint8_t program[] = {
        OP_FADD, R0, R1, R2,
        OP_HALT
    };
    VM vm(program);

    SECTION("0 + 0") {
        float val = 0.0f;
        vm.setRegister(R1, *((uint32_t*)&val));
        vm.setRegister(R2, *((uint32_t*)&val));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t*)&val));
    }

    SECTION("357.34 + 847.21") {
        float val1 = 357.34f;
        float val2 = 847.21f;
        float expected = 1204.55f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t*)&val1));
        vm.setRegister(R2, *((uint32_t*)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t*)&expected));
    }

    SECTION("-3 + -5") {
        float val1 = -3.0f;
        float val2 = -5.0f;
        float expected = -8.0f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t*)&val1));
        vm.setRegister(R2, *((uint32_t*)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t*)&expected));
    }
}

TEST_CASE("OP_SUB") {
    uint8_t program[] = {
        OP_SUB, R0, R1, R2,
        OP_HALT
    };
    VM vm(program);

    SECTION("0 - 0") {
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("1123497651 - 987513") {
        vm.reset();
        vm.setRegister(R1, 1123497651);
        vm.setRegister(R2, 987513);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 1122510138);
    }

    SECTION("0 - 1") {
        vm.reset();
        vm.setRegister(R1, 0);
        vm.setRegister(R2, 1);
        vm.run();
        REQUIRE(vm.getRegister(R0) == UINT32_MAX);
    }
}

TEST_CASE("OP_FSUB") {
    uint8_t program[] = {
        OP_FSUB, R0, R1, R2,
        OP_HALT
    };
    VM vm(program);

    SECTION("0 - 0") {
        float val = 0.0f;
        vm.setRegister(R1, *((uint32_t*)&val));
        vm.setRegister(R2, *((uint32_t*)&val));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t*)&val));
    }

    SECTION("357.34 - 847.21") {
        float val1 = 357.34f;
        float val2 = 847.21f;
        float expected = -489.87f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t*)&val1));
        vm.setRegister(R2, *((uint32_t*)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float*)&actual), expected));
    }

    SECTION("-3 - -5") {
        float val1 = -3.0f;
        float val2 = -5.0f;
        float expected = 2.0f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t*)&val1));
        vm.setRegister(R2, *((uint32_t*)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t*)&expected));
    }
}

TEST_CASE("OP_MUL") {
    uint8_t program[] = {
        OP_MUL, R0, R1, R2,
        OP_HALT
    };
    VM vm(program);

    SECTION("1 * 0") {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("2 * 2") {
        vm.reset();
        vm.setRegister(R1, 2);
        vm.setRegister(R2, 2);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 4);
    }

    SECTION("347 * 987513") {
        vm.reset();
        vm.setRegister(R1, 347);
        vm.setRegister(R2, 987513);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 342667011);
    }
}

TEST_CASE("OP_IMUL") {
    uint8_t program[] = {
        OP_IMUL, R0, R1, R2,
        OP_HALT
    };
    VM vm(program);

    SECTION("1 * 0") {
        vm.setRegister(R1, 1);
        vm.setRegister(R2, 0);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 0);
    }

    SECTION("2 * 2") {
        vm.reset();
        vm.setRegister(R1, 2);
        vm.setRegister(R2, 2);
        vm.run();
        REQUIRE(vm.getRegister(R0) == 4);
    }

    SECTION("-3 * -5") {
        int32_t val1 = -3;
        int32_t val2 = -5;
        int32_t expected = 15;
        vm.reset();
        vm.setRegister(R1, *((uint32_t*)&val1));
        vm.setRegister(R2, *((uint32_t*)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t*)&expected));
    }
}

TEST_CASE("OP_FMUL") {
    uint8_t program[] = {
        OP_FMUL, R0, R1, R2,
        OP_HALT
    };
    VM vm(program);

    SECTION("1 * 0") {
        float val1 = 1.0f;
        float val2 = 0.0f;
        vm.setRegister(R1, *((uint32_t*)&val1));
        vm.setRegister(R2, *((uint32_t*)&val2));
        vm.run();
        REQUIRE(vm.getRegister(R0) == *((uint32_t*)&val2));
    }

    SECTION("357.34 * 847.21") {
        float val1 = 357.34f;
        float val2 = 847.21f;
        float expected = 302742.0214f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t*)&val1));
        vm.setRegister(R2, *((uint32_t*)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float*)&actual), expected));
    }

    SECTION("-3 * -5") {
        float val1 = -3.0f;
        float val2 = -5.0f;
        float expected = 15.0f;
        vm.reset();
        vm.setRegister(R1, *((uint32_t*)&val1));
        vm.setRegister(R2, *((uint32_t*)&val2));
        vm.run();
        uint32_t actual = vm.getRegister(R0);
        REQUIRE(_ALMOST_EQUAL(*((float*)&actual), expected));
    }
}
