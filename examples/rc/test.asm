
call  .main
halt
.main:
    push  r0
    push  r1
    push  r5
    push  ra
    push  bp
    mov  bp, sp
    lconsw  r1, 16
    sub  sp, sp, r1
    lcons  r0, 0
    lconsw  r5, 16
    sub  r5, bp, r5
    stor_p  r5, r0
    lcons  r0, 2
    lconsw  r5, 4
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_J0SXET:
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 10000
    pop  r1
    jl  r1, r0, .loc_BR1P2H
    lconsb  r0, 0
    jmp  .loc_R8JJWL
.loc_BR1P2H:
    lconsb  r0, 1
.loc_R8JJWL:
    jz  r0, .loc_3ZSTD3
    lcons  r0, 1
    lconsw  r5, 12
    sub  r5, bp, r5
    stor_p  r5, r0
    lcons  r0, 2
    lconsw  r5, 8
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_79XX2T:
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    pop  r1
    jl  r1, r0, .loc_ZKL7RB
    lconsb  r0, 0
    jmp  .loc_Y5F7FI
.loc_ZKL7RB:
    lconsb  r0, 1
.loc_Y5F7FI:
    jz  r0, .loc_XS7W3O
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    pop  r1
    imod  r0, r1, r0
    push  r0
    lcons  r0, 0
    pop  r1
    je  r1, r0, .loc_HV9WQW
    lconsb  r0, 0
    jmp  .loc_VAYX89
.loc_HV9WQW:
    lconsb  r0, 1
.loc_VAYX89:
    jz  r0, .loc_OB9VZD
    lcons  r0, 0
    lconsw  r5, 12
    sub  r5, bp, r5
    stor_p  r5, r0
    jmp  .loc_XS7W3O
.loc_OB9VZD:
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    inc  r0
    stor_p  r5, r0
    jmp  .loc_79XX2T
.loc_XS7W3O:
    lconsw  r5, 12
    sub  r5, bp, r5
    load_p  r0, r5
    jz  r0, .loc_8V5S92
    lconsw  r5, 16
    sub  r5, bp, r5
    load_p  r0, r5
    inc  r0
    stor_p  r5, r0
.loc_8V5S92:
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    inc  r0
    stor_p  r5, r0
    jmp  .loc_J0SXET
.loc_3ZSTD3:
    lconsw  r5, 16
    sub  r5, bp, r5
    load_p  r0, r5
    printi  r0, 1
    lcons  r0, 0
    mov  t0, r0
    mov  sp, bp
    pop  bp
    pop  ra
    pop  r5
    pop  r1
    pop  r0
    ret
    lconsb  r0, 0
    mov  t0, r0
    mov  sp, bp
    pop  bp
    pop  ra
    pop  r5
    pop  r1
    pop  r0
    ret