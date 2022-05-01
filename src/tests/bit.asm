	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000
Start
	lda #$c0
	ldx #$ef
	ldy #$ae
	sta $f0
	bit $f0
	org $FFFC
	.word Start
	.word Start
