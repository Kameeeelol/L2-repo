.ORIG x3000

AND R2, R2, #0 ; initialisé a 0
LD R4, iteration
LD R1, nZero ; R1 = '-30'

fin 
IN
ADD R0, R0, R1
ADD R2, R2, R2
ADD R3, R2, R2
ADD R3, R3, R3
ADD R2, R3, R2
ADD R2, R2, R0
ADD R4, R4, #-1
BRP fin

ADD R0, R2, #0

HALT 
iteration .FILL #4
nZero .FILL #-30
.END