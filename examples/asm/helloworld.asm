    ; jump over the data, to our entry label
    jmp         .entry 

$hello          byte[]  "Hello world!", 0x0A

; label definition
.entry: 
    prints      $hello
    halt