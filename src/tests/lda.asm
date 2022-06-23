	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000
Start
	; Immediate
	lda #20
	; Zero Page
	sta $f0
	lda $f0
	; Zero Page X
	ldx #1
	sta $f0,x
	lda $f0,x
	
	org $FFFC
	.word Start
	.word Start
