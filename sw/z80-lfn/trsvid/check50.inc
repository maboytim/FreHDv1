






















check_50:
	di
	in	a,(0ffh)
	ld	b,a		
	and	~40h
	out	(0ech),a	
	ld	a,4
	out	(0e0h),a	
	in	a,(0ech)	
	ld	hl,0
.50wt0:	in	a,(0e0h)
	and	4
	jp	nz,.50wt0	
	in	a,(0ech)	
.50wt1:	in	a,(0e0h)
	and	4
	inc	hl
	jp	nz,.50wt1	
	ld	a,b
	out	(0ech),a	

	ld	a,9
	cp	h		
	ret
