; https://www.c64-wiki.com/index.php/Assembler-Example
; This program rapidly updates the colors
; of the screen and the border.

*=$c000   ; starting address of the program

SCREEN = $d021
BORDER = $d020

start   inc SCREEN  ; increase screen colour 
        inc BORDER  ; increase border colour
        jmp start   ; repeat
