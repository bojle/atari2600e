	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000
Start
	lda #$a4
	sta COLUBK
	lda #$30
	sta COLUPF
	lda #$1e
	sta COLUP0
	
	sta WSYNC
	
	lda #0
	sta CTRLPF
	lda #$10
	sta PF0
	lda #$00
	sta PF1
	sta PF2
	jmp Start
	org $FFFC
	.word Start
	.word Start
