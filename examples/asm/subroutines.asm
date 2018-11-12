
.main:
    call    .sub1
    pop     r1
    print   r1, 1
    halt

.sub1:
    push    ra
    push    r0

    call    .sub2
    mov     r2, r0

    pop     r0
    pop     ra

    push    r2
    ret

.sub2:
    lcons   r0, 1
    ret
