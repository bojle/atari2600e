; a moving dot by Kirk Israel

	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include vcs.h
	include macro.h
	org $F000



;we start by setting up two "variables"
;this means we tell DASM that when we say
;variablename, we mean this specific memory
;location (we have $80 to $FF to play with)

;we'll use this one to store the vertical position
YPosFromBot = $80;
;more on the use of this variable below
VisibleMissileLine = $81;

Start

;generic start up stuff from macro.h...
	CLEAN_START



	lda #$00
	sta COLUBK
	lda #66		;Lets go for purpley!
	sta COLUP0

	lda #80
	sta YPosFromBot	;set Initial Y Position

	;NUSIZ0 sets the size and duplication
	;of the sprite and missiles --see the Stella
	;guide for details
	lda #$20
	sta NUSIZ0 ;Quad Width for now


;VSYNC time
MainLoop
	lda  #2
	sta  VSYNC
	sta  WSYNC
	sta  WSYNC
	sta  WSYNC
	lda  #43
	sta  TIM64T
	lda #0
	sta  VSYNC

	;#% is a way of indicating a binary actual number
	;(just like #$ starts a hex number and # a decimal number)

	lda #%00010000  ;put value of 1 in the left nibble (slow move right)
	sta HMM0	;set the move for missile 0

WaitForVblankEnd
	lda INTIM
	bne WaitForVblankEnd
	ldy #191
	sta WSYNC
	sta VBLANK

	sta WSYNC
	sta HMOVE

;main scanline loop...
ScanLoop
	sta WSYNC

; here the idea is that VisibleMissileLine
; is zero if the line isn't being drawn now,
; otherwise it's however many lines we have to go

; there are more efficient ways of doing this


; we see if this is the line (line # stored in Y) is the
; one that we start the missile on
CheckActivateMissile
	cpy YPosFromBot		;compare Y to the YPosFromBot...
	bne SkipActivateMissile ;if not equal, skip this...
	lda #8			;otherwise say that this should go
	sta VisibleMissileLine	;on for 8 lines
SkipActivateMissile

;turn missile off then see if it's turned on
	lda #0
	sta ENAM0
;
;if the VisibleMissileLine is non zero,
;we're drawing it
;
	lda VisibleMissileLine	;load the value of what missile line we're showing
	beq FinishMissile	;if zero we aren't showing, skip it
IsMissileOn
	lda #2			;otherwise
	sta ENAM0		;showit
	dec VisibleMissileLine 	;and decrement the missile line thing
FinishMissile

	dey		;decrement scanline counter
	bne ScanLoop	;lather rinse repeat


;overscan same as last time
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
