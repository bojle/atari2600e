	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000
Start
	lda #$8e
	sta COLUBK
; TIM1T
	lda #1
	sta TIM1T
loop1
	lda INTIM
	bne loop1

; TIM8T
	lda #1
	sta TIM8T
loop2
	lda INTIM
	bne loop2

; TIM64T
	lda #1
	sta TIM64T
loop3
	lda INTIM
	bne loop3

; T1024T
	lda #$ff
	sta T1024T
loop4
	lda INTIM
	bne loop4

	org $FFFC
	.word Start
	.word Start
