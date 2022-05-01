	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000

yes
	lda #100
	rts

no
	lda #0
	bne end

Start
	lda #0
	jsr yes
end
	lda #10

	org $FFFC
	.word Start
	.word Start
