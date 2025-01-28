.ORIGx3000

    LEA R1, s1
    LEA R2, s2

loop
    LDR R0, R1, #0
    STR R0, R2, #0
    ADD R1, R1, #1
    ADD R2, R2, #1
    ADD R0, R0, #0
    BRnp loop
    HALT

s1 .STRINGZ "kamel"
s2 .BLKW #6
    .END
