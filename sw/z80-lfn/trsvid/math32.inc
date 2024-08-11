





















add32:	add	hl,de
	exx
	adc	hl,de
	exx
	ret

sub32:	or	a
	sbc	hl,de
	exx
	sbc	hl,de
	exx
	ret





divmod32:
	exx
	ld	hl,0
	exx
	ld	hl,0

	ld	a,b	
	ld	b,32
_div32lp:
	sl1	c
	rla
	exx
	rl	c
	rl	b
	exx
	adc	hl,hl
	exx
	adc	hl,hl
	exx

	sbc	hl,de
	exx
	sbc	hl,de
	exx
	jr	nc,_div32bit


	add	hl,de
	exx
	adc	hl,de
	exx
	dec	c

_div32bit:
	djnz	_div32lp
	ld	b,a
	ret



mul32:
	exx
	ld	h,b
	ld	l,c
	exx
	ld	h,b
	ld	l,c

	ld	a,32
_mul32lp:
	add	hl,hl
	exx
	adc	hl,hl
	exx
	rl	e
	rl	d
	exx
	rl	e
	rl	d
	exx

	jr	nc,_mul32noadd

	add	hl,bc
	exx
	adc	hl,bc
	exx


	jr	nc,_mul32noc
	inc	de
_mul32noc:

_mul32noadd:
	dec	a
	jr	nz,_mul32lp
	ret

ld32	macro	reg16,n
	ld	reg16,[n] & 0ffffh
	exx
	ld	reg16,[n] shr 16
	exx
	endm

ld32mem	macro	reg16,m
	ld	reg16,(m)
	exx
	ld	reg16,(m+2)
	exx
	endm

st32	macro	m,reg16
	ld	(m),reg16
	exx
	ld	(m+2),reg16
	exx
	endm


push32	macro	reg16
	exx
	push	reg16
	exx
	push	reg16
	endm

pop32	macro	reg16
	pop	reg16
	exx
	pop	reg16
	exx
	endm

ld32reg	macro	r0,r1,r2,r3
	ld	r0,r2
	ld	r1,r3
	exx
	ld	r0,r2
	ld	r1,r3
	exx
	endm

ld32_bc_de macro
	ld32reg	b,c,d,e
	endm

ld32_bc_hl macro
	ld32reg	b,c,h,l
	endm

ld32_de_bc macro
	ld32reg	d,e,b,c
	endm

ld32_de_hl macro
	ld32reg	d,e,h,l
	endm

ld32_hl_bc macro
	ld32reg	h,l,b,c
	endm

ld32_hl_de macro
	ld32reg	h,l,d,e
	endm
