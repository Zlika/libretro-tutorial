; Point d'entrée du programme
.start

; == Gestion de la couleur du carré ==
; Chargement dans R1 de l'état des boutons
LD R1,$0x1F40
; Bouton A enfoncé ?
CMP R1,0x0100
BEQ .btn_a
; Bouton B enfoncé ?
CMP R1,0x0200
BEQ .btn_b
; Bouton X enfoncé ?
CMP R1,0x0300
BEQ .btn_x
; Bouton Y enfoncé ?
CMP R1,0x0400
BEQ .btn_y
; Aucun bouton enfoncé : couleur blanche
LD R1,0xFFFF
ST R1,.couleur
BRA .start_frame
; Bouton A enfoncé : couleur rouge
.btn_a
LD R1,0xF800
ST R1,.couleur
BRA .start_frame
; Bouton B enfoncé : couleur jaune
.btn_b
LD R1,0xFFE0
ST R1,.couleur
BRA .start_frame
; Bouton X enfoncé : couleur bleue
.btn_x
LD R1,0x001F
ST R1,.couleur
BRA .start_frame
; Bouton Y enfoncé : couleur verte
.btn_y
LD R1,0x07E0
ST R1,.couleur

.start_frame
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

