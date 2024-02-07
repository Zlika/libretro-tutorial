; Lecture d'une musique en boucle
; Point d'entrée du programme
.start
; Do
LD R1,0x0100
ST R1,0x1F60
; Attente de 200ms
LD R1,0
.sleep_note1
CMP R1,50000
BEQ .note2
ADD R1,1
BRA .sleep_note1
; Mi
.note2
LD R1,0x0300
ST R1,0x1F60
; Attente de 200ms
LD R1,0
.sleep_note2
CMP R1,50000
BEQ .note3
ADD R1,1
BRA .sleep_note2
; Sol
.note3
LD R1,0x0500
ST R1,0x1F60
; Attente de 200ms
LD R1,0
.sleep_note3
CMP R1,50000
BEQ .start
ADD R1,1
BRA .sleep_note3
