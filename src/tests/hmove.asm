	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000
Start
	lda #$e0
	sta HMP0
	sta WSYNC
	sta HMOVE

	org $FFFC
	.word Start
	.word Start
