# RISVM

RISVM is a RISC-like bytecode virtual machine written in C++.

It includes a simple Python 3 [assembler](assembler/) and related [examples](assembler/examples).

This project is built as a learning experience and aims for simplicity over optimization. It won't be the fastest VM, but it should run well on any little-endian platform.

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

Currently, data resides together with the program so care must be taken to ensure execution flow never reaches data sections.

Examples:

```assembly
$val32           dword       1234567
$val16           word        1234
$doneStr         byte[]      "done", '!', 0xA
```

### Instructions

Instructions codes are always one-byte long, optionally followed by bytes representing operands. Destinations (if any) are always the first operand.

Listing below (incomplete).

#### System

```assembly
nop                   ; do nothing
```

```assembly
halt                  ; terminate execution
```

```assembly
syscall 0xFF          ; run syscall 0xFF
```

```assembly
int 0xFF              ; trigger interrupt 0xFF
```

#### Registers

```assembly
lcons r0, 0xFFFFFFFF  ; load 32-bit constant
```

```assembly
lconsw r0, 0xFFFFF    ; load 16-bit (word) constant
```

```assembly
lconsb r0, 0xFFF      ; load 8-bit (byte) constant
```

```assembly
mov r0, r1            ; copy the contents of register r1 to r0
```

#### Stack

```assembly
push r0               ; push a register onto the stack
```

```assembly
pop r0                ; pop a value from the stack into a register
```

```assembly
pop2 r0, r1           ; pop two values from the stack into two registers
```

```assembly
dop                   ; duplicate the last value in the stack
```

#### Functions

```assembly
call .mySub           ; set the return address register and jump to label
```

```assembly
ret                   ; return to the address of last caller
```

#### Memory

```assembly
stor $var1, r0        ; store the 32-bit value of r0 in memory location var1
```

```assembly
storw $var1, r0       ; store the 16-bit value of r0 in memory location var1
```

```assembly
storb $var1, r0       ; store the 8-bit value of r0 in memory location var1
```

```assembly
load r0, $var1        ; load the 32-bit value of memory location var1 into r0
```

```assembly
loadw r0, $var1       ; load the 16-bit value of memory location var1 into r0
```

```assembly
loadb r0, $var1       ; load the 8-bit value of memory location var1 into r0
```

```assembly
memcpy $dest, $src, 0xFFFF  ; copy the specified number of bytes from source to dest
```

#### Arithmetic

```assembly
inc r0                ; increment the value of register
```

```assembly
finc r0               ; (float) increment the value of register
```

```assembly
dec r0                ; decrement the value of register
```

```assembly
fdec r0               ; (float) decrement the value of register
```

```assembly
add r0, r1, r2        ; add r1 and r2 and store result in r0
```

```assembly
fadd r0, r1, r2       ; (float) add r1 and r2 and store result in r0
```

```assembly
sub r0, r1, r2        ; subtract r2 from r1 and store result in r0
```

```assembly
fsub r0, r1, r2       ; (float) subtract r2 from r1 and store result in r0
```

```assembly
mul r0, r1, r2        ; multiply r1 by r2 and store result in r0
```

```assembly
imul r0, r1, r2       ; (signed) multiply r1 by r2 and store result in r0
```

```assembly
fmul r0, r1, r2       ; (float) multiply r1 by r2 and store result in r0
```

```assembly
div r0, r1, r2        ; divide r1 by r2 and store result in r0
```

```assembly
idiv r0, r1, r2       ; (signed) divide r1 by r2 and store result in r0
```

```assembly
fdiv r0, r1, r2       ; (float) divide r1 by r2 and store result in r0
```

```assembly
shl r0, r1, r2        ; shift r1 left by r2 and store result in r0
```

```assembly
shr r0, r1, r2        ; shift r1 right by r2 and store result in r0
```

```assembly
ishr r0, r1, r2       ; (signed) shift r1 right by r2 and store result in r0
```

```assembly
mod r0, r1, r2        ; store remainder of r1 divided by r2 in r0
```

```assembly
imod r0, r1, r2       ; (signed) store remainder of r1 divided by r2 in r0
```

```assembly
and r0, r1, r2        ; store reult of r1 AND r2 in r0
```

```assembly
or r0, r1, r2         ; store reult of r1 OR r2 in r0
```

```assembly
xor r0, r1, r2        ; store reult of r1 XOR r2 in r0
```

```assembly
not r0, r1            ; store reult of NOT r1 in r0
```

## License

Licnesed under the MIT License, see the [LICENSE](LICENSE) file for details.
