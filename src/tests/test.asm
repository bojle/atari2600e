; thin red line by Kirk Israel

	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include "vcs.h"
	org $F000
Start
	sei
	cld
	ldx #$FF
	txs
	lda #0
ClearMem
	sta 0,X
	dex
	bne ClearMem
	lda #$66
	sta COLUBK
	lda #$d0
	sta COLUP0
MainLoop
	lda  #2
	sta  VSYNC
	sta  WSYNC
	sta  WSYNC
	sta  WSYNC
	sta VBLANK
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	lda #0
	sta  VSYNC
	ldy #191

	sta WSYNC
	sta VBLANK
	lda #$F0
	sta HMM0

	sta WSYNC
	sta HMOVE
ScanLoop
	sta WSYNC
	sta ENAM0
	dey
	bne ScanLoop

	lda #2
	sta WSYNC
	sta VBLANK
	ldx #30
OverScanWait
	sta WSYNC
	dex
	bne OverScanWait
	jmp  MainLoop

	org $FFFC
	.word Start
	.word Start
