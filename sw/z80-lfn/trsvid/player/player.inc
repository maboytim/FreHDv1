;
; player.inc - common definitions and code for players
;

audbuf		equ	$fe00 - (4 * 3 - 4)
audbuf_end	equ	audbuf + 12
timestamp	equ	$fe00 - 2
cmd_off		equ	2	; so L == 2 or $82 giving 0 V cassette output.

f_data		equ	$c2

 block_start	$d000+cmd_off	; CMD3_EOF
 	pop	bc	; get rid of our pause
	pop	bc	; and get rid of generic pause
	ret
 block_end

 block_start	$d100+cmd_off	; underflow
 	ld	a,'U'
	ld	($3fff),a
; It may not matter because if we're reading bad data here we're in trouble.
; But allowing for an abort via keyboard would be useful.
uflw:	in	a,(f_data)
	cp	254
	jr	nz,uflw
	jp	false_underflow

play:	ld	hl,$feff
	ld	(hl),$d1

 	ld	c,f_data
	ld	hl,$3c00
	ld	de,audbuf
	exx
	ld	b,1
	ld	c,a			; audio port passed in
	ld	de,audbuf_end

	ld	hl,pause
	push	hl

	ld	l,cmd_off
	in	a,(f_data)
	ld	h,a
	jp	(hl)

; Have our own pause routine as we need to finish off the interrupted HOME cmd.
pause:	pop	iy		; get common pause address
	dec	sp
	dec	sp
	dec	sp
	dec	sp		; get stack to what it normally is

	ld	hl,(timestamp)	; tell pause routine where we are
	call	jpiy		; call out to common pause routine
; Now finish off where HOME was interrupted
	in	l,(c)
	in	h,(c)
	exx
	in	a,(f_data)
	ld	h,a
	jp	(hl)
jpiy:	jp	(iy)
 block_end

 block_start	$d180+cmd_off	; underflow
false_underflow:
	exx
	ld	de,audbuf
	exx
	ld	e,low(audbuf_end)
	ld	l,cmd_off		; just in case
	in	h,(c)
	jp	(hl)
 block_end

	block_header	play
