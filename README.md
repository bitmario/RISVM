# RISVM - Reduced Instruction Set VM

RISVM is a RISC-like bytecode virtual machine written in C++.

It includes a simple Python 3 [assembler](assembler/) and related [examples](assembler/examples).

This project is built as a learning experience and aims for simplicity over optimization. It won't be the fastest VM, but it should run well on any little-endian platform.

## Getting started

Build the standard VM interpreter and assemble an example:

```bash
make
python3 assembler/assembler.py assembler/examples/helloworld.asm
```

Run it!

```
./vm assembler/examples/helloworld.bin
```

### Embedding

Include `vm.h` in your project and do something like this:

```cpp
uint8_t program[] = { ... };
VM vm(program); 
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
    printp      $hello
    halt
```

### Instruction reference

#### System

```assembly
nop                   ; do nothing
halt                  ; terminate execution
syscall 0xFF          ; run syscall 0xFF
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
storw $var1, r0       ; store the 16-bit value of r0 in memory location var1
storb $var1, r0       ; store the 8-bit value of r0 in memory location var1
load r0, $var1        ; load the 32-bit value of memory location var1 into r0
loadw r0, $var1       ; load the 16-bit value of memory location var1 into r0
loadb r0, $var1       ; load the 8-bit value of memory location var1 into r0
memcpy $dest, $src, 0xFFFF  ; copy the specified number of bytes from source to dest
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
print r0, 0x1          ; print register, with or without newline (0/1)
printi r0, 0x1         ; (signed) print register, with or without newline (1/0)
printf r0, 0x1         ; (float) print register, with or without newline (1/0)
printp $myStr          ; (char array) print string at address
println                ; print a newline
```

## License

Licnesed under the MIT License, see the [LICENSE](LICENSE) file for details.
