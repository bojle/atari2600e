	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000
Start
	lda #0
	sta WSYNC
	org $FFFC
	.word Start
	.word Start
