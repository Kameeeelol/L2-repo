; AIT MOHAMMED ARIS, 12411021 ;
; SEGGARI KAMAL 12203821 ;


    .ORIG x3000

    IN ; lecture d'un caractère

    ; verifier si le caractère donné est entre 0 et 9 
    ; verifier si il est <= neuf 
    LD R1, neuf ; charger neuf dans R1 
    NOT R1, R1 
    ADD R1, R1, #1
    ADD R2, R0, R1 ; verifier si R0 <= 9 
    BRp nonchiffre; saute vers non chiffre 


    ; verifier si il est >= a 0
    LD R1, zero ; charger zero dans R1 
    NOT R1, R1 
    ADD R1, R1, #1
    ADD R2, R0, R1 ; verifier si R0 >= 0
    BRn nonchiffre ; saute vers nonchiffre

    ; une fois verifier que c'est un chiffre, mettons le en chiffre decimal 
    AND R1, R1, #0 ; initiliser R1 a 0 
    LD R2, zero ; charger zero dans R2
    NOT R2, R2 
    ADD R2, R2, #1 ; complement a deux 
    ADD R0, R0, R2 ; R0 = le chiffre donné en caractère

    ;verifions si le chiffre est pair ou impair 
    AND R1, R0, #1 ; mettre tout les bits a zero sauf le dernier bits de poids faible 
    BRz pair
    LD R0, un ; stocker la valeur 1 dans R0
    OUT ; l'afficher sur l'écran 
    BR fin ; sauter vers la fin 

pair 
    LD R0, zero ; stocker zero dans R0
    OUT ; l'afficher sur l'ecran 
    BR fin ; sauter vers la fin 

nonchiffre
    LD R0, interro ; stocker la valeur de interro dans R0
    OUT 

fin

    HALT
interro .FILL x3F
neuf .FILL x39
zero .FILL x30 
un .FILL x31

    .END 