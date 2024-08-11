







	include	getmodel.inc
	include	check50.inc
	include	check_m1_wait.inc

hwinfo: call	getmodel
	push	af
	cp	3
	ld	b,60
	call	nc,getFPS
	pop	af		
	push	af		
	push	bc		
	cp	4
	ld	d,0
	call	nc,get_m1_wait
	pop	bc
	ld	c,d
	pop	af
	cp	4
	ret	c		

	ld	a,1
	out	(84h),a
	out	(9ch),a		
	ld	hl,0
	push	bc
	call	memtype
	pop	bc
	ld	a,4
	ret	z		
	xor	a
	out	(9ch),a		
	ld	a,5
	ret






memtype:
	ld	b,(hl)
	ld	a,b
	cpl
	ld	(hl),a
	xor	(hl)		
	ld	(hl),b
	ret

getFPS:	call	check_50
	ld	b,60
	ret	nz
	ld	b,50
	ret



get_m1_wait:
	ld	a,50
	cp	b
	push	af		
	call	check_m1_wait
	ld	d,a
	pop	af
	ret	nz		
	inc	d		
	ret
