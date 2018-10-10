
.main:
    call    .sub
    printi  ax
    halt

.sub:
    lcons   ax, 1
    ret
