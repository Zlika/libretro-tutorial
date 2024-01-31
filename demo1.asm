; Point d'entrée du programme
.start
; Chargement de la position initiale dans la mémoire vidéo
LD R1,.init_video_pos
ST R1,.video_pos
; y = 0
LD R1, 0
ST R1, .cnt_y

; Boucle sur l'axe Y
.boucle_y
; x = 0
LD R1,0
ST R1, .cnt_x
; Boucle sur l'axe X
.boucle_x
; Chargement de la couleur dans R2
LD R2,.couleur
; Chargement de l'adresse en mémoire vidéo dans R1
LD R1,.video_pos
; Stockage de la couleur à l'adresse mémoire courante de la mémoire vidéo
ST R2,R1
; Incrémentation de la position dans la mémoire vidéo
ADD R1,2
ST R1,.video_pos
; Fin de la ligne courante ?
LD R1,.cnt_x
CMP R1,9
BEQ .new_line
; Incrémentation de l'offset x
ADD R1,1
ST R1,.cnt_x
BRA .boucle_x
.new_line
; Incrémentation de l'offset y
LD R2,.cnt_y
ADD R2,1
ST R2,.cnt_y
; Fin du carré de 10px par 10px ?
CMP R2,10
BEQ .start
; Incrémentation de la position dans la mémoire vidéo
; (avance de 80-10 pixels = 70*2 octets = 140 octets)
LD R2,.video_pos
ADD R2,140
ST R2,.video_pos
BRA .boucle_y

; ===== Déclaration des variables =====

; Réservation d'espace en mémoire pour stocker la position linéaire
; initiale du carré de 10pixels de côté dans la mémoire vidéo
.init_video_pos
DW 3270
; Réservation d'espace en mémoire pour stocker la couleur (blanc = 0XFFFF)
.couleur
DW 0xFFFF
; Réservation d'espace en mémoire pour stocker la position linéaire 
; dans la mémoire vidéo
.video_pos
DW 0x0000
; Réservation d'espace en mémoire pour stocker le compteur X (de 0 à 9)
.cnt_x
DW 0x0000
; Réservation d'espace en mémoire pour stocker le compteur Y (de 0 à 9)
.cnt_y
DW 0x0000

