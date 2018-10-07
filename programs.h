#ifndef __PROGRAMS_H__
#define __PROGRAMS_H__

#include "vm.h"

const uint8_t program[] = {
    LCONSW, DX, 0x4, 0x0, // loop end
    LCONSW, CX, 0x0, 0x0, // loop counter
    // start:
    LCONSW, AX, 0x5, 0x0,    // first operand
    LCONSW, BX, 0x6, 0x0,    // second operand
    ADD, AX, BX,             //
    LCONSW, BX, 0x2, 0x0,    //
    DIV, AX, BX,             //
    PUSH, AX,                //
    POP, AX,                 //
    INCR, CX,                // increment counter
    JNE, CX, DX, 0x08, 0x00, // jump to start if not equal
    A_AW, 0x2, 0x10, 0x00,
    HALT //
};

const uint8_t program2[] = {
    LCONS, DX, 0xAF, 0x21, 0x23, 0x02,
    PUSH, DX,
    POP, DX,
    STOR, 0x02, 0x00, DX,
    MEMCPY, 0x04, 0x00, 0x02, 0x00, 0x0A, 0x00,
    LOAD, BX, 0x0A, 0x00,
    PUSH, BX,
    POP, BX,
    LCONSW, AX, 0xA0, 0x00,
    CSTORS_R, AX, 'H', 'i', '!', 0x00,
    PRINT, AX,
    PRINTLN,
    I2S, DX, 0xB0, 0x00,
    S2I, 0xB0, 0x00, EX,
    PUSH, EX,
    POP, EX,
    LCONS, FX, 0x46, 0x94, 0x28, 0x41, // 10.5362
    LCONS, AX, 0x40, 0x94, 0x02, 0x48, // 133713
    PRINTF, FX,
    PRINTLN,
    PRINTF, AX,
    PRINTLN,
    ADDF, AX, FX,
    PRINTF, AX,
    PRINTLN,
    MOV, BX, AX,
    F2I, BX,
    PRINTI, BX,
    PRINTLN,
    HALT //
};

const uint8_t benchmark[] = {
    LCONSB, AX, 2,
    LCONSB, BX, 3,

    LCONSB, CX, 0,
    LCONS, DX, 0x80, 0xF0, 0xFA, 0x02, // 50 000 000

    MULT, AX, BX,
    DIV, AX, BX,
    INCR, AX,
    INCR, BX,

    INCR, CX,
    JNE, CX, DX, 0X0F, 0X00,
    HALT};
// CPP (around 10 times faster):
// #include <stdint.h>
// int main() {
//     int32_t a = 2;
//     int32_t b = 3;
//     for (int32_t i=0; i<50000000; i++) {
//         a *= b;
//         a /= b;
//         a++;
//         b++;
//     }
//     return 0;
// }

const uint8_t program3Data[] = {
    0xE8, 0x03,         // int16 at 0x00 0x00
    'H', 'i', '!', 0x00 // cstring at 0x02 0x00
};
const uint8_t program3[] = {
    LOADW, AX, 0x00, 0x00,
    PRINTI, AX,
    PRINTLN,
    LCONSW, BX, 0x02, 0x00,
    PRINT, BX,
    PRINTLN,
    HALT};

#endif // __PROGRAMS_H__