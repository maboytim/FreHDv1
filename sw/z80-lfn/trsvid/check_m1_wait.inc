










check_m1_wait:
	di
	in	a,(0ffh)
	ld	b,a
	or	a,40h
	out	(0ech),a	
	xor	a
	out	(0e0h),a	
	ld	a,4
	out	(0e0h),a	
	in	a,(0ech)	

	ld	hl,0
.wt0:	in	a,(0e0h)
	and	4
	jp	nz,.wt0
	in	a,(0ech)	
.wt1:	in	a,(0e0h)
	and	4
	inc	hl
	jp	nz,.wt1
	in	a,(0ech)	
	ld	a,b
	out	(0ech),a	




	ld	a,l
	add	a,100
	ld	a,h
	adc	a,-6
	neg
	add	a,2
	ret
