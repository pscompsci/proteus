; Fibonacci number generator program

; First two fibonacci numbers
    push    0
    push    1
loop:
    dup     1
    dup     1
    add
    jmp     loop
