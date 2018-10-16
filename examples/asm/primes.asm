; calculate primes below 100000
; runtime: 4,966s
    lconsb  r0, 1
    lcons   r1, 100000

.loop:
    lconsb  r2, 2

    .innerLoop:
        jae     r2, r0, .isPrime

        mod     r3, r0, r2
        jz      r3, .loopEnd

        inc     r2
        jmp     .innerLoop

.isPrime:
    print   r0, 1

.loopEnd:
    inc     r0
    jb      r0, r1, .loop

.end:
;    prints      $doneStr
    halt

;$doneStr       byte[]      "done", '!', 0xA

; C++ equivalent
; runtime: 1,800s (G++ 7.3.0 -O3)

; #include <stdio.h>
; #include <stdint.h>
; int main() {
;     for (uint32_t num=1; num<100000; num++) {
;         bool isPrime = true;
;         for (uint32_t i=2; i<num; i++) {
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


; JS (node v8.10.0)
; 2,175s (wow)

; for (num=1; num<100000; num++)
; {
;   isPrime = true;
;   for (i=2; i<num; i++)
;   {
;     if (num % i == 0)
;     {
;       isPrime = false;
;       break;
;     }
;   }

;   if (isPrime)
;     console.log(`${num}`);
; }


; Lua 5.3
; runtime: 7,234s

; for num=1,99999 do
;   isPrime = true
;   for i=2,num-1 do
;     if num % i == 0 then
;       isPrime = false
;       break
;     end
;   end
;   if isPrime then
;     print(num)
;   end
; end


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
