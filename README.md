# RISVM - Reduced Instruction Set VM

[![Build Status](https://travis-ci.com/bitmario/RISVM.svg?branch=master)](https://travis-ci.com/bitmario/RISVM) [![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

RISVM is a low overhead, embeddable bytecode virtual machine with a RISC architecture which is designed to work on any little-endian platform with GCC or clang.

It is implemented in around 1000 lines of simple C++ and includes [tests](test/) and a basic Python 3 [assembler](assembler/) with [documentation](#assembly) and [examples](examples/asm).

This project is still a work in progress, so be aware that backwards compatibility may be broken at any time.

## Getting started

Build the standard VM interpreter and assemble an example:

```bash
make
python3 assembler/assembler.py examples/asm/helloworld.asm
```

Run it!

```
./vm examples/asm/helloworld.bin
```

### Embedding

Include `vm.h` in your project and do something like this:

```cpp
uint8_t program[] = { ... };
VM vm(program, sizeof(program)); 
vm.run();
```

## Architecture

### Registers

There are 16 general-purpose and 4 special registers, all of which are stored as 32-bit unsigned integers.

| name  | number | type                             | preserved |
|-------|--------|----------------------------------|-----------|
| r0-r5 | 0-5    | general-purpose                  | Y         |
| t0-t9 | 6-15   | general-purpose (temporary/args) | N         |
| ip    | 16     | instruction pointer              | Y         |
| bp    | 17     | base pointer                     | Y         |
| sp    | 18     | stack pointer                    | Y         |
| ra    | 19     | return address                   | N         |

### Memory

Currently, data resides together with the program so care must be taken to ensure execution flow never reaches data sections. All labels and data references are made using positive 16-bit offsets from the first program byte.

A stack is also available, currently hardcoded to 128 `uint32` values.

Please note that there are currently no checks for stack overflows or out of bounds memory access so there will be undefined behavior if those happen. This is one of the reasons why you shouldn't run untrusted programs or use this for critical applications at this stage.

### Instructions

Instruction codes are always one-byte long and, depending on type, may be followed by one or more bytes a representing operands. Instructions are also not aligned, in order to save space.

In RISC spirit, there are only 70 [instructions](#instruction-reference), most of which are quite simple and operate only on registers (except load/store instructions and string operations).

## Assembly

Instruction names are inspired on x86 which makes them sound familiar and allows us to leverage standard assembly highlighting in editors.

Destinations (if any) are always the first operand.

### Constants

Numerical constants can be specified as `123`, `-123` or `0xA1`. They will always be encoded as unsigned unless they are negative.

Single-character constants can be specified with single quotes, e.g. `'A'`.

Strings can be specified with double quotes, e.g. `"A short string"`.

### Defining data

Data and respective labels may be defined by specifying a name prefixed with `$`, a type (`byte` for 8 bits, `word` for 16 bits or `dword` for 32 bits) and the value. To specity an array, append brackets to the type and separate values with commas.

If a string is included in a `byte[]` definition, then a null terminator byte will automatically be appended to the byte array. If you build your string from individual characters, you must add the null terminator yourself.

Examples:

```assembly
$val32           dword       1234567
$val16           word        1234
$doneStr         byte[]      "done", '!', 0xA
```

### Defining labels

Labels pointing to specific addresses in the program can be defined by starting a line with a period, followed by an identifier and a colon. Labels must be defined in their own line.

These labels can then be used as jump targets, e.g.:

```assembly
    ; jump over the data, to our entry label
    jmp         .entry 

$hello          byte[]  "Hello world!", 0x0A

; label definition
.entry: 
    prints      $hello
    halt
```

### Instruction reference

#### System

```assembly
nop                   ; do nothing
halt                  ; terminate execution
int 0xFF              ; trigger interrupt 0xFF
```

#### Registers

```assembly
lcons r0, 0xFFFFFFFF  ; load 32-bit constant
lconsw r0, 0xFFFF     ; load 16-bit (word) constant
lconsb r0, 0xFF       ; load 8-bit (byte) constant
mov r0, r1            ; copy the contents of register r1 to r0
```

#### Stack

```assembly
push r0               ; push a register onto the stack
pop r0                ; pop a value from the stack into a register
pop2 r0, r1           ; pop two values from the stack into two registers
dup                   ; duplicate the last value in the stack
```

#### Functions

```assembly
call .mySub           ; set the return address register and jump to label
ret                   ; return to the address of last caller
```

#### Memory

```assembly
stor $var1, r0        ; store the 32-bit value of r0 in memory location var1
stor_p r1, r0         ; store the 32-bit value of r0 in the memory location pointed by r1
storw $var1, r0       ; store the 16-bit value of r0 in memory location var1
storw_p r1, r0        ; store the 16-bit value of r0 in the memory location pointed by r1
storb $var1, r0       ; store the 8-bit value of r0 in memory location var1
storb_p r1, r0        ; store the 8-bit value of r0 in the memory location pointed by r1
load r0, $var1        ; load the 32-bit value of memory location var1 into r0
load_p r0, r1         ; load the 32-bit value of the memory location pointed by r1 into r0
loadw r0, $var1       ; load the 16-bit value of memory location var1 into r0
loadw_p r0, r1        ; load the 16-bit value of the memory location pointed by r1 into r0
loadb r0, $var1       ; load the 8-bit value of memory location var1 into r0
loadb_p r0, r1        ; load the 8-bit value of the memory location pointed by r1 into r0
memcpy $dest, $src, 0xFFFF  ; copy the specified number of bytes from source to dest
memcpy_p r0, r1, r2   ; copy the # bytes in r2 from the address in r1 to the address in r0
```

#### Arithmetic

```assembly
inc r0                ; increment the value of register
finc r0               ; (float) increment the value of register
dec r0                ; decrement the value of register
fdec r0               ; (float) decrement the value of register
add r0, r1, r2        ; add r1 and r2 and store result in r0
fadd r0, r1, r2       ; (float) add r1 and r2 and store result in r0
sub r0, r1, r2        ; subtract r2 from r1 and store result in r0
fsub r0, r1, r2       ; (float) subtract r2 from r1 and store result in r0
mul r0, r1, r2        ; multiply r1 by r2 and store result in r0
imul r0, r1, r2       ; (signed) multiply r1 by r2 and store result in r0
fmul r0, r1, r2       ; (float) multiply r1 by r2 and store result in r0
div r0, r1, r2        ; divide r1 by r2 and store result in r0
idiv r0, r1, r2       ; (signed) divide r1 by r2 and store result in r0
fdiv r0, r1, r2       ; (float) divide r1 by r2 and store result in r0
shl r0, r1, r2        ; shift r1 left by r2 and store result in r0
shr r0, r1, r2        ; shift r1 right by r2 and store result in r0
ishr r0, r1, r2       ; (signed) shift r1 right by r2 and store result in r0
mod r0, r1, r2        ; store remainder of r1 divided by r2 in r0
imod r0, r1, r2       ; (signed) store remainder of r1 divided by r2 in r0
and r0, r1, r2        ; store reult of r1 AND r2 in r0
or r0, r1, r2         ; store reult of r1 OR r2 in r0
xor r0, r1, r2        ; store reult of r1 XOR r2 in r0
not r0, r1            ; store reult of NOT r1 in r0
```

#### Conversions

```assembly
f2i r0, r1            ; convert float r1 to signed int and store in r0
i2f r0, r1            ; convert signed int r1 to float and store in r0
```

#### Branching

```assembly
jmp .labelName         ; unconditional jump to address
jr r0                  ; unconditional jump to address in register
jz r0, .jmpDest        ; jump if r0 is zero
jnz r0, .jmpDest       ; jump if r0 is not zero
je r0, r1, .jmpDest    ; jump if r0 and r1 are equal
jne r0, r1, .jmpDest   ; jump if r0 and r1 are not equal
ja r0, r1, .jmpDest    ; jump if r0 is greater than r1
jg r0, r1, .jmpDest    ; (signed) jump if r0 is greater than r1
jae r0, r1, .jmpDest   ; jump if r0 is greater or equal to r1
jge r0, r1, .jmpDest   ; (signed) jump if r0 is greater or equal to r1
jb r0, r1, .jmpDest    ; jump if r0 is less than r1
jl r0, r1, .jmpDest    ; (signed) jump if r0 is less than r1
jbe r0, r1, .jmpDest   ; jump if r0 is less or equal to r1
jle r0, r1, .jmpDest   ; (signed) jump if r0 is less or equal to r1
```

#### I/O

```assembly
print r0, 0x1          ; print register r0, with or without newline (0/1)
printi r0, 0x1         ; (signed) print register r0, with or without newline (1/0)
printf r0, 0x1         ; (float) print register r0, with or without newline (1/0)
printc r0              ; (char) print single character from a code in r0
prints $myStr          ; (char array) print string at address
println                ; print a newline
read r0                ; read an integer from stdin to r0
readi r0               ; (signed) read a signed integer from stdin to r0
readf r0               ; (float) read a float from stdin to r0
readc r0               ; (char) read a char from stdin to r0
reads $strBuf, 10      ; (char array) read a max of 10 chars from stdin into $strBuf
```

## Performance

While performance is not the main focus, we aim to make the VM as efficient as possible without compromising simplicity.

The [primes.asm](examples/asm/primes.asm) example was used as a benchmark on x86-64 system. Here are the results vs. comparable implementations in some popular languages:

| implementation    | runtime   | performance hit |
|-------------------|-----------|-----------------|
| C++ (G++ 7.3 -O3) | 1.80s     | 0% (baseline)   |
| JS (node 8.10)    | 2.18s     | 21%             |
| **RISVM**         | **4.97s** | **176%**        |
| Lua 5.3           | 7.23s     | 305%            |
| Python 3.6.6      | 35.95s    | 1897%           |

It seems like RISVM is about 3x slower than native code, but still beats most scripting languages. However, on less powerful architectures like Xtensa and AVR the VM can be up to 10 times slower than native code.

## License

Licnesed under the MIT License, see the [LICENSE](LICENSE) file for details.

Includes the [Catch2](https://github.com/catchorg/Catch2) testing library which is licensed under the Boost Software License, read the full license [here](test/lib/catch2_LICENSE).
