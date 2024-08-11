;
; play6 - 160 x 72 full screen video for fast Model 4's.
;
; Code aims to output an audio sample every 256 cycles.  However, writing
; graphics data incurs unpredictable wait states.  At least 1 state and up
; to 4.  We might be able to get a better prediction but it is hampered by
; audio changing when writes happen.  Based on pure random chance we would
; average 2.5 cycles per write and 20 cycles per graphics opcode which writes
; 8 bytes to the screen.  Empirical measurement on the 4P shows that 19
; is a closer estimate.  And there's the slight problem that have 270 audio
; samples per frame which is 6 more than the exact 264.  Seems like I
; could get that down to 265 easily enough but don't want to bother.
;
; So graphics opcodes are only allotted 256 - 19 cycles and that value is
; noted here and used when we construct the code at runtime.  However, if
; it is changed then the gfx_pad_tpl template will have to be adjusted
; accordingly.

screen_write_drag equ 19

tnybuf	equ	$c300

; I could write some fancy macros to help build these tables but
; it is easier overall to write them by hand.

gfx_tpl:
	defb	1,4
		inc	a
	defb	1,4
		exx
	rept	8
	defb	2,16+3
		ini
	endm
	defb	1,4
		exx
	defb	2,12+3
		in	h,(c)
	defb	2,12+3
		in	h,(c)
	defb	0

gfx_pad_tpl:
	defb	1,4
		nop
	defb	2,9
	ld	i,a
	defb	0

aud_tpl:
	defb	1,4
		inc	a
	defb	1,4
		exx
	defb	2,12+3
		in	l,(c)
	defb	2,7
		ld	h,1
	defb	1,7
		ld	e,(hl)
	defb	1,4
		inc	l
	defb	1,7
		ld	h,(hl)
	defb	1,4
		ld	l,e
	defb	2,12+3
		in	d,(c)
	defb	2,12+3
		in	d,(c)
	defb	2,12+3
		in	e,(c)
	defb	2,12+3
		in	d,(c)
	defb	1,4
		exx
	defb	2,12+3
		in	e,(c)
	defb	1,4
		ex	de,hl
	defb	2,12+3
		in	d,(c)
	defb	1,7
		ld	(hl),d
	defb	1,4
		ex	de,hl
	defb	2,12+3
		in	h,(c)
	defb	3,19
		sub	(ix)
	defb	1,5
		ret	c
	defb	0

aud_pad_tpl:
	rept	4
	defb	1,4
	nop
	endm
	defb	3,10
	ld	bc,254*256+f_data
	defb	0

snd_off_tpl:
	defb	2,11
p6p0:		out	($90),a
	defb	0

tail_tpl:
	defb	1,4
		xor	a
	defb	2,11
p6p1:		out	($90),a
	defb	1,4
		jp	(hl)
	defb	0

gfpos	equ	$40

gap_look_tpl:
	rept	7
	defb	2,11+3
		in	a,(f_data)
	defb	1,4
		cp	b
	defb	2,7
		jr	z,$+2+gfpos
	endm

	defb	1,4
		ex	de,hl
	defb	1,7
		ld	d,(hl)
	defb	1,4
		ex	de,hl

	defb	1,4
		dec	e
	defb	2,7
		jr	z,$-7	; patched to underflow
	defb	1,5
		ret	z
	defb	1,5
		ret	z
	defb	2,7
		ld	l,0

	defb	0

; To make the "jr z,$+gfpos" easy in gap_look_tpl, this must match in length
; and cycles/step.

gap_found_tpl:
	rept	7
	defb	2,14
		ld	a,(hl)
		ld	a,(hl)
	defb	1,4
		xor	a
	defb	2,7
		jr	nz,$+2
	endm

	defb	1,4
		ex	de,hl
	defb	1,7
		ld	d,(hl)
	defb	1,4
		ex	de,hl

	defb	1,4
		dec	e
	defb	2,7
		jr	z,$-7	; patch to back to main
	defb	1,5
		ret	z
	defb	1,5
		ret	z
	defb	2,7
		ld	l,gfpos

	defb	0


gap_snd_off_tpl:
	defb	4,4+11+4
		ex	af,af'
p6p2:		out	($90),a
		ex	af,af'
	defb	0

gap_tail_tpl:
	defb	4,4+11+4
		xor	a
p6p3:		out	($90),a
		jp	(hl)
	defb	0

gap_to_main_tpl:
	defb	1,5
		ret	nz
	defb	2,7
		ld	l,0
	defb	4,4+11+4
		xor	a
p6p4:		out	($90),a
		jp	(hl)
	defb	0

gap_underflow_tpl:
	defb	3,10
		jp	underflow
	defb	0


write_all_template:
	ld	a,255		; assume enough cycles for entire template.
write_template:
tpl_lp:	ld	b,(hl)		; get size
	inc	b
	dec	b
	ret	z		; size 0 means end of template
	inc	hl
	cp	(hl)		; cycles_avail - step_cycles
	dec	hl
	ret	c		; instruction doesn't fit, return.
	inc	hl
	sub	(hl)		; take away the cycles of this instruction
	ex	af,af'
	add	(hl)		; count total used
	ex	af,af'
	inc	hl		; update remaining cycle count
copy:	ld	c,(hl)		; output instruction
	inc	hl
	ld	(ix),c
	inc	ix
	djnz	copy
	jr	tpl_lp

player6:
	ld	(p6p0+1),a
	ld	(p6p1+1),a
	ld	(p6p2+1),a
	ld	(p6p3+1),a
	ld	(p6p4+1),a

	push	ix

; Page 1 maps 0 to 254 to screen positions +0 to +1016

	ld	ix,$100
	ld	hl,$f800
	ld	de,16
	ld	b,128

genpos:	ld	(ix),l
	inc	ix
	ld	(ix),h
	inc	ix
	add	hl,de
	djnz	genpos

; Create handlers for codes $02 to $41.  These are the most commonly used
; codes that output 8 graphics bytes, one audio byte and waste a byte.

	ld	ixh,$02
gfx_codes:
	ld	a,screen_write_drag
	ex	af,af'

	ld	ixl,0
	ld	a,ixh
	sub	2
	add	a,a
	add	a,a

	ld	hl,gfx_tpl
	call	write_template		; output what will fit
	push	hl
	ld	hl,gfx_pad_tpl
	call	write_template		; output nops for closer fit
	push	hl
	ld	hl,snd_off_tpl
	call	write_all_template	; sound off code
	pop	hl
	call	write_all_template	; rest of nops
	pop	hl
	call	write_all_template	; rest of code
	ld	hl,tail_tpl
	call	write_all_template	; and the finish

	ex	af,af'
	jr	nz,$			; Ooops, gfx didn't add up to 256

	inc	ixh
	ld	a,$42
	cp	ixh
	jr	nz,gfx_codes

; Create handlers for codes $42 to $81.  These ones keep the audio going
; mop up spare bytes and can buffer bytes for sector gap codes.

	ld	ixh,$42
aud_codes:
	xor	a
	ex	af,af'

	ld	ixl,0
	ld	a,ixh
	sub	$42
	add	a,a
	add	a,a

	ld	hl,aud_tpl
	call	write_template
	push	hl
	ld	hl,aud_pad_tpl
	call	write_template
	push	hl
	ld	hl,snd_off_tpl
	call	write_all_template
	pop	hl
	call	write_all_template
	pop	hl
	call	write_all_template
	ld	hl,tail_tpl
	call	write_all_template

	ex	af,af'
	jr	nz,$			; Ooops, aud didn't add up to 256

	inc	ixh
	ld	a,$82
	cp	ixh
	jr	nz,aud_codes

; Create handlers for codes $82 to $C1.  These ones handle skipping over
; the CRC and variable gap between sectors.

	ld	ixh,$82
gap_codes:
	xor	a
	ex	af,af'

	ld	ixl,0
	ld	a,ixh
	sub	$82
	add	a,a
	add	a,a

	ld	hl,gap_look_tpl
	call	write_template
	push	hl
	ld	hl,gap_snd_off_tpl
	call	write_all_template
	pop	hl
	call	write_all_template
	ld	hl,gap_tail_tpl
	call	write_all_template

	ex	af,af'
	jr	nz,$			; Oops, gap didn't add up to 256

	push	ix			; Save branch target

	ld	hl,gap_underflow_tpl
	call	write_all_template

; now fix up branch

	pop	hl
	ld	e,l			; E is branch destination
	ld	b,e			; also is the number of bytes to check
	ld	l,0

fix1:	ld	a,(hl)
	cp	$f7			; $f7 RST 7 which we won't use
	jr	nz,nofix1
	ld	a,e
	sub	l
	dec	a
	ld	(hl),a			; make it branch to E
nofix1:	inc	hl
	djnz	fix1

	xor	a
	ex	af,af'

	ld	ixl,gfpos
	ld	a,ixh
	sub	$82
	add	a,a
	add	a,a

	ld	hl,gap_found_tpl
	call	write_template
	push	hl
	ld	hl,gap_snd_off_tpl
	call	write_all_template
	pop	hl
	call	write_all_template
	ld	hl,gap_tail_tpl
	call	write_all_template

	ex	af,af'
	jr	nz,$			; Ooops, gap didn't add up to 256

	push	ix

	ld	hl,gap_to_main_tpl
	call	write_all_template

	pop	hl
	ld	e,l
	ld	l,gfpos
	ld	a,l
	sub	e
	ld	b,a

fix2:	ld	a,(hl)
	cp	$f7			; won't use RST 7
	jr	nz,nofix2
	ld	a,e
	sub	l
	dec	a
	ld	(hl),a
nofix2:	inc	hl
	djnz	fix2

	inc	ixh
	ld	a,$c2
	cp	ixh
	jp	nz,gap_codes

; EOF handler
	ld	hl,$c1c1		; POP BC ; POP BC
	ld	($c200),hl
	ld	a,$c9			; RET
	ld	($c202),a

	pop	ix

	ld	hl,play6
	ld	(play+1),hl

	ret

play6:
	ld	ix,$f7ff

 	ld	c,f_data
	ld	hl,$f800
	exx

	ld	hl,pause6
	push	hl

	ld	d,high(tnybuf)
	ld	b,254
	ld	c,f_data
	ld	hl,$2200		; first command hardcoded

	ld	a,1
	ex	af,af'
	xor	a

	jp	(hl)

; We've been going through the CRC and $FF stuff bytes looking for the $FE
; that indicates the the next sector follows.  But this has taken longer than
; we allow so there's nothing for it but to keep looking for the sector start
; and continuing on as we would in the gap_found_tpl.

underflow:
	ld	a,'U'
	ld	($f800 + 24 * 80 - 1),a

; It may not matter because if we're reading bad data here we're in trouble.
; But allowing for an abort via keyboard would be useful.

uflw:	in	a,(f_data)
	cp	254
	jr	nz,uflw

	ld	hl,$2200		; hardcoded GFX command when we continue
	jp	(hl)

; Don't permit pause unless we're at the top a frame.

no_pause:
	exx
	jp	(hl)

; When a key is pressed we know that DE' has the current second count.
; Also, HL is where to continue.
 
pause6:	pop	iy		; get common pause address
	dec	sp
	dec	sp
	dec	sp
	dec	sp		; stack back to normal

	exx
	ld	a,h
	cp	$f8
	jr	nz,no_pause
	ld	a,l
	or	a
	jr	nz,no_pause

	push	de		; save timestamp
	exx
	ex	(sp),hl		; save HL, get timestamp
	call	jpiy		; call general-purpose pause
	pop	hl		; restore HL
	jp	(hl)		; continue where we left off
jpiy:	jp	(iy)
