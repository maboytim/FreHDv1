



getmodel:
	in	a,(0ffh)	
	ld	b,a		
	ld	c,60h
	xor	c		
	out	(0ech),a	
	in	a,(0ffh)	
	xor	c		
	xor	b		
	ld	c,0ech
	out	(c),b		
	rlca
	rlca
	jr	nc,.ism4		
	rlca
	ld	a,3
	ret	nc		
	ld	a,1		
	ret
.ism4:	ld	a,4
	ret
