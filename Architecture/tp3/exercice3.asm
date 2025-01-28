.ORIG x3000

LD R1, nZero ; R1= '-30'

; premier chiffre 
IN
ADD R2, R0, R1 ; R2: '1er carac'-30 == <1erCarac>

; deuxieme chiffre
IN
ADD R0, R0, R1 ; <2em Carac>
ADD R2, R2, R2 ; <1erCarac> * 2
ADD R3, R2, R2 ; <1erCarac> * 4
ADD R3, R3, R3 ; <1erCarac> * 8
ADD R2, R3, R2 ; <1erCarac> * 10
ADD R2, R2, R0 ; <c1c2>

; troisième chiffre
IN
ADD R0, R0, R1 ;<c3>
ADD R2, R2, R2 ; <c1c2> * 2
ADD R3, R2, R2 ; <c1c2> * 4
ADD R3, R3, R3 ; <c1c2> * 8
ADD R2, R3, R2 ; <c1c2> * 10
ADD R2, R2, R0 ; <c1c2c3>

; quatriemme chiffre 
IN
ADD R0, R0, R1 ;<c4>
ADD R2, R2, R2 ; <c1c2c3> * 2
ADD R3, R2, R2 ; <c1c2c3> * 4
ADD R3, R3, R3 ; <c1c2c3> * 8
ADD R2, R3, R2 ; <c1c2c3> * 10
ADD R2, R2, R0 ; <c1c2c3c4>

ADD R0, R2, #0
OUT

HALT
nZero .FILL x-30
.END