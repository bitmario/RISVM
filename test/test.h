#ifndef __TEST_H__
#define __TEST_H__

#include <math.h>
#include "lib/catch.hpp"
#include "../src/vm.h"

#define _U32_GARBAGE 0xF1E2D3C4U
#define _U16_GARBAGE 0xF1E2U
#define _U8_GARBAGE 0xF1U

#define _NTH_BYTE(num, n) (num >> (8 * n)) & 0xFF

#define _ALMOST_EQUAL(x, y) fabs(x - y) < 0.0001

#endif // __TEST_H__