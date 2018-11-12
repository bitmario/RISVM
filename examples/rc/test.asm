
call  .main
printi  t0, 1
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
.loc_KPQEMM:
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 100000
    pop  r1
    jl  r1, r0, .loc_O71WHV
    lconsb  r0, 0
    jmp  .loc_ICLSH9
.loc_O71WHV:
    lconsb  r0, 1
.loc_ICLSH9:
    jz  r0, .loc_02I413
    lcons  r0, 1
    lconsw  r5, 12
    sub  r5, bp, r5
    stor_p  r5, r0
    lcons  r0, 2
    lconsw  r5, 8
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_3FRN9B:
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    pop  r1
    jl  r1, r0, .loc_S2OWLV
    lconsb  r0, 0
    jmp  .loc_39HGJG
.loc_S2OWLV:
    lconsb  r0, 1
.loc_39HGJG:
    jz  r0, .loc_R20GKA
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
    je  r1, r0, .loc_E3ODPX
    lconsb  r0, 0
    jmp  .loc_3WZ5I6
.loc_E3ODPX:
    lconsb  r0, 1
.loc_3WZ5I6:
    jz  r0, .loc_SQZ9EW
    lcons  r0, 0
    lconsw  r5, 12
    sub  r5, bp, r5
    stor_p  r5, r0
    jmp  .loc_R20GKA
.loc_SQZ9EW:
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    inc  r0
    stor_p  r5, r0
    jmp  .loc_3FRN9B
.loc_R20GKA:
    lconsw  r5, 12
    sub  r5, bp, r5
    load_p  r0, r5
    jz  r0, .loc_JTMFSD
    lconsw  r5, 16
    sub  r5, bp, r5
    load_p  r0, r5
    inc  r0
    stor_p  r5, r0
.loc_JTMFSD:
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    inc  r0
    stor_p  r5, r0
    jmp  .loc_KPQEMM
.loc_02I413:
    lconsw  r5, 16
    sub  r5, bp, r5
    load_p  r0, r5
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