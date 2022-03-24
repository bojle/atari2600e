; move a happy face with the joystick by Kirk Israel
; (with a can't'dodge'em line sweeping across the screen)

	processor 6502
	incdir "/home/metal/warez/machines/atari2600"
	include vcs.h
	include macro.h
	org $F000

YPosFromBot = $80;
VisiblePlayerLine = $81;

Start
	CLEAN_START

	lda #$00   ;start with a black background
	sta COLUBK
	lda #$1C   ;lets go for bright yellow, the traditional color for happyfaces
	sta COLUP0
;Setting some variables...
	lda #80
	sta YPosFromBot	;Initial Y Position

;; Let's set up the sweeping line. as Missile 1


	lda #2
	sta ENAM1  ;enable it
	lda #33
	sta COLUP1 ;color it

	lda #$20
	sta NUSIZ1	;make it quadwidth (not so thin, that)


	lda #$F0	; -1 in the left nibble
	sta HMM1	; of HMM1 sets it to moving


;VSYNC time
MainLoop
	lda #2
	sta VSYNC
	sta WSYNC
	sta WSYNC
	sta WSYNC
	lda #43
	sta TIM64T
	lda #0
	sta VSYNC


;Main Computations; check down, up, left, right
;general idea is to do a BIT compare to see if
;a certain direction is pressed, and skip the value
;change if so

;
;Not the most effecient code, but gets the job done,
;including diagonal movement
;

; for up and down, we INC or DEC
; the Y Position

	lda #%00010000	;Down?
	bit SWCHA
	bne SkipMoveDown
	inc YPosFromBot
SkipMoveDown

	lda #%00100000	;Up?
	bit SWCHA
	bne SkipMoveUp
	dec YPosFromBot
SkipMoveUp

; for left and right, we're gonna
; set the horizontal speed, and then do
; a single HMOVE.  We'll use X to hold the
; horizontal speed, then store it in the
; appropriate register


;assum horiz speed will be zero
	ldx #0

	lda #%01000000	;Left?
	bit SWCHA
	bne SkipMoveLeft
	ldx #$10	;a 1 in the left nibble means go left

;; moving left, so we need the mirror image
	lda #%00001000   ;a 1 in D3 of REFP0 says make it mirror
	sta REFP0

SkipMoveLeft
	lda #%10000000	;Right?
	bit SWCHA
	bne SkipMoveRight
	ldx #$F0	;a -1 in the left nibble means go right...

;; moving right, cancel any mirrorimage
	lda #%00000000
	sta REFP0

SkipMoveRight


	stx HMP0	;set the move for player 0, not the missile like last time...



; see if player and missile collide, and change the background color if so

	;just a review...comparisons of numbers always seem a little backwards to me,
	;since it's easier to load up the accumulator with the test value, and then
	;compare that value to what's in the register we're interested.
	;in this case, we want to see if D7 of CXM1P (meaning Player 0 hit
	; missile 1) is on. So we put 10000000 into the Accumulator,
	;then use BIT to compare it to the value in CXM1P

	lda #%10000000
	bit CXM1P
	beq NoCollision	;skip if not hitting...
	lda YPosFromBot	;must be a hit! load in the YPos...
	sta COLUBK	;and store as the bgcolor
NoCollision
	sta CXCLR	;reset the collision detection for next time




WaitForVblankEnd
	lda INTIM
	bne WaitForVblankEnd
	ldy #191


	sta WSYNC
	sta HMOVE

	sta VBLANK


;main scanline loop...


ScanLoop
	sta WSYNC

; here the idea is that VisiblePlayerLine
; is zero if the line isn't being drawn now,
; otherwise it's however many lines we have to go

CheckActivatePlayer
	cpy YPosFromBot
	bne SkipActivatePlayer
	lda #8
	sta VisiblePlayerLine
SkipActivatePlayer



;set player graphic to all zeros for this line, and then see if
;we need to load it with graphic data
	lda #0
	sta GRP0

;
;if the VisiblePlayerLine is non zero,
;we're drawing it now!
;
	ldx VisiblePlayerLine	;check the visible player line...
	beq FinishPlayer		;skip the drawing if its zero...
IsPlayerOn
	lda BigHeadGraphic-1,X	;otherwise, load the correct line from BigHeadGraphic
				;section below... it's off by 1 though, since at zero
				;we stop drawing
	sta GRP0		;put that line as player graphic
	dec VisiblePlayerLine 	;and decrement the line count
FinishPlayer


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


; here's the actual graphic! If you squint you can see its
; upsidedown smiling self
BigHeadGraphic
	.byte #%00111100
	.byte #%01111110
	.byte #%11000001
	.byte #%10111111
	.byte #%11111111
	.byte #%11101011
	.byte #%01111110
	.byte #%00111100

	org $FFFC
	.word Start
	.word Start
