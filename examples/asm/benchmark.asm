; counter
    lconsb      r0, 0
; loop end
    lcons       r1, 2000000
; divider
    lconsb      r2, 13
    lconsb      r4, 0

.loopStart:
    mov         r3, r0
    mod         r3, r2

    jnz         r3, .loopEnd
    
    printi      r0
    println

.loopEnd:
    inc         r0
    jb          r0, r1, .loopStart

    
    halt