


















	jrpromote 0

w0_HL_overhead	equ	93
w0_A_overhead	equ	74

_w0_256:
	dec	h		
	ld	a,256-4-4-12-4-7-17-w0_A_overhead
				
	call	w0_A		
w0_HL:	inc	h		
	dec	h		
	jr	nz,_w0_256	
	ld	a,l		
w0_A:	srl	a		
	jr	c,.b1		
	nop			
.b1:	rrca			
	jr	nc,.b2		
	and	07fh		
.b2:	rra			
	jp	nc,.b3		
	or	a		
.b3:	rra			
	jp	nc,.b4		
	or	a		
	nop			
.b4:	ret	z		
.lp16:	dec	a		
	jr	nz,.lp16	
	ret	z		



w1_HL_overhead	equ	171
w1_A_overhead	equ	113

w1_HL:	ld	a,l		
	call	.wa		
	or	h		
	jp	z,_ret		


.256:	ld	a,256-w1_A_overhead-8-42	
	call	.wa		
	dec	h		
	jp	nz,.256		
	ret			

.wa:	ld	l,0		

			    
w1_A:	rrca			
	jr	nc,.b0		
	ret	nc		

.b0:	rrca			
	jr	nc,.b1		
	inc	hl		

.b1:	rrca			
	jr	nc,.b2a		
	inc	hl		
.b2a:	jr	nc,.b2b		
	inc	hl		

.b2b:	rrca			
	jp	nc,.b3		
	or	a,0		

.b3:	and	a,0fh		
	jp	z,_ret		

.lp:	dec	a		
	jp	nz,.lp		
_ret:	ret			

w2_HL_overhead	equ	158
w2_A_overhead	equ	131

_w2_256:		   
	dec	h		
	ld	a,256-60-w2_A_overhead	
	call	w2_A		

w2_HL:	inc	h		
	dec	h		
	jr	nz,_w2_256	
	ld	a,l		

			   
w2_A:	rrca			
	jr	nc,.0c		
	nop			

.0c:	rrca			
	jr	nc,.1c		
	ret	nc		

.1c:	rrca			
	jr	nc,.2c		
	jr	nc,.2c		

.2c:	rrca			
	jr	nc,.3c		
	ret	nc		
	nop			

.3c:	rrca			
	jp	nc,.4c		
	inc	hl		
	dec	hl		

.4c:	and	a,7		

.lp32:	jp	z,_ret		
	dec	a		
	jr	.lp32		








w0_time	macro	label,st,en
label	equ	t(en)-t(st)
	endm

call_w0_HL macro
	call	w0_HL
	tstate	t($)+w0_HL_overhead
	endm

call_w0_A macro
	call	w0_A
	tstate	t($)+w0_A_overhead
	endm

w1_time	macro	label,st,en
label	equ	t(en)-t(st)+[ocf(en)-ocf(st)]
	endm

call_w1_HL macro
	call	w1_HL
	tstate	t($)+w1_HL_overhead
	endm

call_w1_A macro
	call	w1_A
	tstate	t($)+w1_A_overhead
	endm

w2_time	macro	label,st,en
label	equ	t(en)-t(st)+2*[ocf(en)-ocf(st)]
	endm

call_w2_HL macro
	call	w2_HL
	tstate	t($)+w2_HL_overhead
	endm

call_w2_A macro
	call	w2_A
	tstate	t($)+w2_A_overhead
	endm
