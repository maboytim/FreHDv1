










sdecHL:
sprtHL:	bit	7,h
	jr	z,decHL
	ld	a,'-'
	call	putc
	ld	de,0
	ex	de,hl
	or	a
	sbc	hl,de
decHL:
prtHL:	ld	b,0		
	ld	de,10000
	call	_pdig
	ld	de,1000
	call	_pdig
	ld	de,100
	call	_pdig
	ld	de,10
	call	_pdig
	ld	b,1		
	ld	de,1

_pdig:	xor	a
_fndig:	add	a,1		
	sbc	hl,de
	jr	nc,_fndig
	add	hl,de
	add	a,'0'-1
	bit	0,b
	jp	nz,putc		
	cp	'0'
	ret	z		
	ld	b,1		
	jp	putc
