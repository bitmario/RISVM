; testing
    jmp     .entry
.start:
    mul     r2, r2, r0
    printi  r2
    println

    inc     r0
    jne     r0, r1, .start

    halt
.entry:
    lconsb  r0, 1
    lconsb  r1, 11
    lconsb  r2, 2
    jmp     .start
