



	include	frehdef.inc

frdchk:	in	a,($ff)		
	ld	b,a
	or	a,$10		
	out	($ec),a		
	ld	a,getver
	out	(f_cmd),a
	ex	(sp),hl
	ex	(sp),hl		
idwait:	in	a,(f_stat)
	cp	$ff		
	jr	z,nofrd
	rlca
	jr	c,idwait
	in	a,(f_size)
	cp	6
	jr	nz,nofrd
	ld	a,b
	out	($ec),a		
	ld	a,1		
	ret
nofrd:	ld	a,b
	out	($ec),a		
	xor	a		
	ret

wait:	ex	(sp),hl
	ex	(sp),hl
wait1:	in	a,(f_stat)
	rlca
	jr	c,wait1		
	in	a,(f_stat)
	and	1
	jp	nz,frehd_error
	ret



read_timestamp:
	ld	a,gettime
	out	(f_cmd),a
	call	wait
	ld	b,6
	ld	c,f_data
	inir
	ret

	include	math32.inc




conv_clock:
	push	hl
	pop	ix
	ld32	bc,0
	ld	c,(ix+2)	
	ld32	de,60
	call	mul32		
	ld32	de,0
	ld	e,(ix+1)	
	call	add32		
	ld32_bc_hl
	ld32	de,60
	call	mul32		
	ld32	de,0
	ld	e,(ix+0)
	call	add32		
	ret















diff_clock:
	push	hl
	ex	de,hl
	call	conv_clock	
	pop	de
	push32	hl
	ex	de,hl
	call	conv_clock	
	pop32	de
	call	sub32		
	ret	nc		
	ld32	de,86400
	call	add32		
	ret
