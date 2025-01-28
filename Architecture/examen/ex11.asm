    .ORIG x3000

    LEA R1, s1
    LEA R2, s2

loop LDR R3, R1, #0
     LDR R4, R2, #0
     NOT R4, R4
     ADD R4, R4, #1
     ADD R0, R3, R4
     BRnp noneq

    ADD R3, R3, #1
    BRz end 

    ADD R1, R1, #1
    ADD R2, R2, #1
    BR loop

noneq AND R0, R0, #0
end HALT
s1  .STRINGZ "KAMEL"
s2 .STRINGZ "KAMEL"
    .END