; counter
    lconsb      ax, 0
; loop end
    lcons       bx, 2000000
; divider
    lconsb      cx, 13
    lconsb      ex, 0

.loopStart:
    mov         dx, ax
    mod         dx, cx

    jnz         dx, .loopEnd
    
    printi      ax
    println

.loopEnd:
    inc         ax
    jl          ax, bx, .loopStart

    
    halt