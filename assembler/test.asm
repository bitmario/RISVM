; testing
    jmp     .entry
.start:
    mult    cx, ax
    printi  cx
    println

    incr    ax
    jne     ax, bx, .start

    halt
.entry:
    lconsb  ax, 1
    lconsb  bx, 11
    lconsb  cx, 2
    jmp     .start
