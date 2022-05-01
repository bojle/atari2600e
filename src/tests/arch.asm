	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000
Start
	ldx #192
	lda #0
	ldy #$ae
Scan
	sta WSYNC
	sta PF0
	sta PF1
	sta PF2
	sta COLUBK
	sty COLUPF
	clc
	adc #1
	dex
	bne Scan

	org $FFFC
	.word Start
	.word Start
