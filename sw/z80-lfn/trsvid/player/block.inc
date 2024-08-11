;
; block.inc - macros for wrapping code or data blocks
;

bk_end	defl	0

block_start	macro	addr
	if	bk_end > 0
		assert	addr > bk_end	; make sure blocks are consecutive
		defb	addr - bk_end	; distance to previous block
	else
		defb	0		; this is the last block
	endif
	phase	addr
bk_beg	defl	$
	endm

block_end	macro
bk_end	defl	$
	dephase
	defb	bk_end - bk_beg	; block length
	endm

block_header	macro	entry
	defw	bk_end - 1	; address of last byte
	defw	entry
	endm
