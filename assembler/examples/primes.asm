; calculate primes below 100000
; runtime: 12,944s
    lconsb  ax, 1
    lcons   bx, 100000

.loop:
    lconsb  cx, 2

    .innerLoop:
        jge     cx, ax, .isPrime

        mov     dx, ax
        mod     dx, cx
        jz      dx, .loopEnd

        inc     cx
        jmp     .innerLoop

.isPrime:
    printi  ax
    println

.loopEnd:
    inc     ax
    jl      ax, bx, .loop

.end:
    halt


; C++ equivalent
; runtime: 1,805s (G++ 7.3.0)

; #include <stdio.h>
; #include <stdint.h>
; int main() {
;     for (int32_t num=1; num<100000; num++) {
;         bool isPrime = true;
;         for (int32_t i=2; i<num; i++) {
;             if (num % i == 0) {
;                 isPrime = false;
;                 break;
;             }
;         }
;         if (isPrime)
;             printf("%d\n", num);
;     }
;     return 0;
; }


; Python 3.6.6
; runtime: 35,952s

; for num in range(1, 100001):
;     is_prime = True
;     for i in range(2, num):
;         if num % i == 0:
;             is_prime = False
;             break
;     if is_prime:
;         print(num)
