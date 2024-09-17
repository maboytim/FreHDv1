;;;
;;; Copyright (C) 2013 Frederic Vecoven
;;;
;;; This file is part of trs_hard
;;;
;;; trs_hard is free software; you can redistribute it and/or modify
;;; it under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 3 of the License, or
;;; (at your option) any later version.
;;;
;;; trs_hard is distributed in the hope that it will be useful,
;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License for more details.
;;;
;;; You should have received a copy of the GNU General Public License
;;; along with this program. If not, see <http://www.gnu.org/licenses/>.
;;;

	list	p=18f47q83
	#include	<xc.inc>
	#include	"bootloader.inc"
	#include	"trs_hard_defs.h"
	#include	"version.h"

	;; extern udata
	global	_sector_buffer, _extra_buffer, _loader_code

	;; extern udata access
	global	_state_status, _state_wp, _state_control
	global	_state_bytesdone, _state_bytesdone2
	global	_state_command, _state_command2, _state_size2, _state_error2
	global	_state_busy, _val_1F, _foo
	global	_state_error, _state_cyl, _state_seccnt, _state_secnum
	global	_state_drive, _state_head, _state_present
	global	_state_secsize, _state_secsize16
	global	_state_rom, _state_romdone
	global	_action_type, _action_flags
	

#define	TRS_ADDR	PORTA
#define TRS_DATA_IN	PORTD
#define TRS_DATA_OUT	LATD	
#define TRS_DATA_TRIS	TRISD
#define GAL_INT_IF	PIR6,0
#define GAL_INT_IE	PIE6,0	
#define GAL_INT		PORTB,1
#define TRS_WAIT	LATB,3
#define STAT_CS		LATC,0
#define GLED		LATB,7
#define RLED		LATB,6


		psect	appintdata,class=UDATA
save_fsr0:	ds	3
		

;;;
;;; High priority interrupt code
;;;
;;; - placed at a fixed address APP_HI_INT, because the vector is in the
;;;   bootloader area.
;;;
;;; - everything is written in assembly code for speed. C is too slow,
;;;   specially when functions must be called. To achieve this, the
;;;   state variables have been placed in access memory ("near" in C).
;;;
;;; - if something takes too long, it is done in the main loop by executing
;;;   an action. During that time, the TRS must poll the status register and
;;;   wait until the action completes
;;; 

	psect	appintcode,class=CODE,space=0,reloc=2
	;org	APP_HI_INT

handle_int2:
	GLOBAL	handle_int2

	bcf	GAL_INT_IF,c	; acknowledge the interrupt

	tstfsz	BANKMASK(_state_busy),c	; state_busy should always be 0
	bra	handle_busy	; (unless TRS forgot to poll STATUS)

	movf	TRS_ADDR,w,c	; dispatch based on the TRS address
	andlw	0x1F		; PORTA = A3 A2 A1 A0 R/W
	cpfseq	BANKMASK(_val_1F),c		; is it CF ? (write command)
	bra	jj		; no, just jump
	swapf	TRS_DATA_IN,w,c
	andlw	0x0F		; lower nibble = command
	movwf	BANKMASK(_state_command),c	; save command here (save instructions later)
	clrf	BANKMASK(_state_bytesdone+0),c
	clrf	BANKMASK(_state_bytesdone+1),c
	addlw	0x1F		; add jump offset
jj:	call	jump
	

	goto	trs_read_wp	    ; 	0 R C0 192
	goto	trs_read_data	    ; 	1 R C8 200
	goto	trs_read_rom	    ; 	2 R C4
	goto	trs_read_cyllo	    ; 	3 R CC 204
	goto	trs_read_data2	    ; 	4 R C2 194
	goto	trs_read_seccnt	    ; 	5 R CA 202
	goto	trs_read_uart_status ; 	6 R C6 
	goto	trs_read_sdh	    ; 	7 R CE 206
	goto	trs_read_control    ; 	8 R C1 193
	goto	trs_read_error	    ; 	9 R C9 201
	goto	trs_read_error2	    ; 	A R C5
	goto	trs_read_cylhi	    ; 	B R CD 205
	goto	trs_read_size2	    ; 	C R C3 195
	goto	trs_read_secnum	    ; 	D R CB 203
	goto	trs_read_uart	    ; 	E R C7
	goto	int_ret_rd	    ; 	F R CF 207 (74HC595)
	
	goto	int_ret_rd	     ; 	0 W C0
	goto	trs_write_data	     ; 	1 W C8 200
	goto	trs_write_command2   ; 	2 W C4 196
	goto	trs_write_cyllo	     ; 	3 W CC 204
	goto	trs_write_data2	     ; 	4 W C2 194
	goto	trs_write_seccnt     ; 	5 W CA 202
	goto	trs_write_uart_ctrl  ; 	6 W C6
	goto	trs_write_sdh	     ; 	7 W CE 206
	goto	trs_write_control    ; 	8 W C1 193
	goto	int_ret_rd	     ; 	9 W C9
	goto	trs_write_rom	     ; 	A W C5
	goto	trs_write_cylhi	     ; 	B W CD 205
	goto	trs_write_size2	     ; 	C W C3 195
	goto	trs_write_secnum     ; 	D W CB 203
	goto	trs_write_uart	     ; 	E W C7
	goto	int_ret_rd	     ; 	F W CF 207
	goto	trs_write_cmd_restore;  F W CF 207 (restore)
	goto	trs_write_cmd_read   ;  F W CF 207 (read)
	goto	trs_write_cmd_write  ;  F W CF 207 (write)
	goto	trs_write_cmd_verify ;  F W CF 207 (verify)
	goto	trs_write_cmd_format ;  F W CF 207 (format)
	goto	trs_write_cmd_init   ;  F W CF 207 (init)
	goto	trs_write_cmd_seek   ;  F W CF 207 (seek)
	goto	trs_write_cmd_nop    ;  F W CF 207 (no operation)
	goto	trs_write_cmd_test   ;  F W CF 207 (test - WD1002)
	goto	trs_write_cmd_nop    ;  F W CF 207 (no operation)
	goto	trs_write_cmd_nop    ;  F W CF 207 (no operation)
	goto	trs_write_cmd_nop    ;  F W CF 207 (no operation)
	goto	trs_write_cmd_nop    ;  F W CF 207 (no operation)
	goto	trs_write_cmd_nop    ;  F W CF 207 (no operation)
	goto	trs_write_cmd_nop    ;  F W CF 207 (no operation)

;;; implement a jump table by messing with the return stack
jump:
	rlncf	WREG,w,c		;  goto instructions are 4 bytes
	rlncf	WREG,w,c
	addwf	TOSL,f,c
	clrf	WREG,c		; NOT NEEDED. jump table above doesn't
	addwfc	TOSH,f,c		; cross page boundary
	addwfc	TOSU,f,c		; cross page boundary
	return

handle_busy:
	btfsc	TRS_ADDR,4,c	; TRS read (0) or write (1) ?
	goto	int_ret_rd	; write => ignore
	movff	_state_status,TRS_DATA_OUT ; read => return status
	goto	int_ret_wr
	
trs_read_wp:
	movff	_state_wp,TRS_DATA_OUT
	goto	int_ret_wr

trs_read_control:
	movff	_state_control,TRS_DATA_OUT
	goto	int_ret_wr
	
trs_read_data:
	movff	FSR0L,save_fsr0+0	; save FSR0
	movff	FSR0H,save_fsr0+1
	lfsr	0,_sector_buffer
	movf	BANKMASK(_state_bytesdone+0),w,c
	addwf	FSR0L,f,c
	movf	BANKMASK(_state_bytesdone+1),w,c
	addwfc	FSR0H,f,c
	movff	INDF0,TRS_DATA_OUT
	infsnz	BANKMASK(_state_bytesdone+0),f,c	; bytesdone++
	incf	BANKMASK(_state_bytesdone+1),f,c
	movf	BANKMASK(_state_bytesdone+0),w,c	; bytesdone == secsize16 ?
	subwf	BANKMASK(_state_secsize16+0),w,c
	bz	bnz1
	goto	int_ret_wr_FSR0		; no, provide data + restore FSR0
bnz1:	movf	BANKMASK(_state_bytesdone+1),w,c
	subwf	BANKMASK(_state_secsize16+1),w,c
	bz	bnz2
	goto	int_ret_wr_FSR0		; no, provide data + restore FSR0
bnz2:	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_READY | TRS_HARD_SEEKDONE)	; done !
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	goto	int_ret_wr_FSR0_S	; provide data + restore FSR0 + status


trs_read_data2:
	movff	FSR0L,save_fsr0+0	; save FSR0
	movff	FSR0H,save_fsr0+1
	lfsr	0,_extra_buffer
	movff	_state_bytesdone2,FSR0L
	incf	BANKMASK(_state_bytesdone2),f,c
	movff	INDF0,TRS_DATA_OUT
	goto	int_ret_wr_FSR0	; provide data + restore FSR0

trs_read_size2:
	movff	_state_size2+0,TRS_DATA_OUT
	goto	int_ret_wr

trs_read_error:
	movff	_state_error,TRS_DATA_OUT
	goto	int_ret_wr

trs_read_error2:
	movff	_state_error2,TRS_DATA_OUT
	goto	int_ret_wr
		
trs_read_secnum:
	movff	_state_secnum,TRS_DATA_OUT
	goto	int_ret_wr
		
trs_read_seccnt:
	movff	_state_seccnt,TRS_DATA_OUT
	bra	int_ret_wr

trs_read_cyllo:
	movff	_state_cyl+0,TRS_DATA_OUT
	bra	int_ret_wr

trs_read_cylhi:
	movff	_state_cyl+1,TRS_DATA_OUT
	bra	int_ret_wr

trs_read_sdh:
	rlncf	BANKMASK(_state_secsize),w,c
	swapf	WREG,w,c
	movwf	BANKMASK(_foo),c
	rlncf	BANKMASK(_state_drive),w,c
	rlncf	WREG,w,c
	rlncf	WREG,w,c
	iorwf	BANKMASK(_state_head),w,c
	iorwf	BANKMASK(_foo),w,c
	movwf	TRS_DATA_OUT,c
	bra	int_ret_wr


trs_read_uart_status: ; 	6 R C6
trs_read_uart:	    ; 	E R C7
	bra	int_ret_rd

trs_write_control:
	movf	TRS_DATA_IN,w,c
	movwf	BANKMASK(_state_control),c
	btfsc	WREG,3,c			;  TRS_HARD_DEVICE_ENABLE = 0x8
	bra	int_ret_rd
	tstfsz	BANKMASK(_state_present),c
	bra	twco0
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_READY | TRS_HARD_SEEKDONE | TRS_HARD_ERR)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	movlw	TRS_HARD_NFERR
	movwf	BANKMASK(_state_error),c
	bra	int_ret_rd_S
twco0:	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_READY | TRS_HARD_SEEKDONE)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),b
	bra	int_ret_rd_S

trs_write_data:
	movf	BANKMASK(_state_command),w,c
	xorlw	(TRS_HARD_WRITE >> 4)
	bnz	int_ret_rd
	movff	FSR0L,save_fsr0+0
	movff	FSR0H,save_fsr0+1
	lfsr	0,_sector_buffer
	movf	BANKMASK(_state_bytesdone+0),w,c
	addwf	FSR0L,f,c
	movf	BANKMASK(_state_bytesdone+1),w,c
	addwfc	FSR0H,f,c
	movff	TRS_DATA_IN,INDF0
	movff	save_fsr0+0,FSR0L	; restore FSR0
	movff	save_fsr0+1,FSR0H
	infsnz	BANKMASK(_state_bytesdone+0),f,c	; bytesdone++
	incf	BANKMASK(_state_bytesdone+1),f,c
	movf	BANKMASK(_state_bytesdone+0),w,c	; bytesdone == secsize16 ?
	subwf	BANKMASK(_state_secsize16+0),w,c
	bnz	int_ret_rd
	movf	BANKMASK(_state_bytesdone+1),w,c
	subwf	BANKMASK(_state_secsize16+1),w,c
	bnz	int_ret_rd
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_BUSY | TRS_HARD_CIP)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	movlw	ACTION_HARD_WRITE
	movwf	BANKMASK(_action_type),c
	bsf	BANKMASK(_action_flags),ACTION_TRS_BIT,c
	bra	int_ret_rd_S

trs_write_data2:
	movff	FSR0L,save_fsr0+0
	movff	FSR0H,save_fsr0+1
	lfsr	0,_extra_buffer
	movff	_state_bytesdone2,FSR0L
	movff	TRS_DATA_IN,INDF0
	movff	save_fsr0+0,FSR0L	; restore FSR0
	movff	save_fsr0+1,FSR0H
	incf	BANKMASK(_state_bytesdone2),f,c
	movf	BANKMASK(_state_size2),w,c
	cpfseq	BANKMASK(_state_bytesdone2),c
	bra	int_ret_rd
	movlw	ACTION_EXTRA2
	bra	action_extraN

;;;
;;; End of interrupt routine. 
;;;
;;; The code is put here so that "bra" instructions can be used above 
;;; and below us.
;;;
;;; Variants: with/without restore FSR0 and with/without update statatus
;;;
int_ret_wr_FSR0_S:
	btfss	PIR3,0,c
	bra	$-2
	bsf	STAT_CS,c
	movf	SPI1RXB,w,b
	bcf	STAT_CS,c
int_ret_wr_FSR0:
	movff	save_fsr0+0,FSR0L	; restore FSR0
	movff	save_fsr0+1,FSR0H
int_ret_wr:
	clrf	TRS_DATA_TRIS,c	; if TRS reads, put data on the bus
int_ret_rd:	
	bsf	TRS_WAIT,c	; tell GAL to release WAIT*
	btfss	GAL_INT,c	; wait transaction completion
	bra	$-2
	setf	TRS_DATA_TRIS,c
	bcf	TRS_WAIT,c	; get ready for next transaction
	retfie	F

;;; with status update
int_ret_rd_S:
	btfss	PIR3,0,c
	bra	$-2
	bsf	STAT_CS,c
	bsf	TRS_WAIT,c	; tell GAL to release WAIT*
	btfss	GAL_INT,c	; wait transaction completion
	bra	$-2
	setf	TRS_DATA_TRIS,c
	movf	SPI1RXB,w,b
	bcf	STAT_CS,c
	bcf	TRS_WAIT,c		; get ready for next transaction
	retfie	F
	
	
trs_read_rom:
	movlw	0x02			; 2nd byte (going to RAM) ?
	cpfseq	BANKMASK(_state_romdone),c
	bra	trr1
	movff	_state_rom,TRS_DATA_OUT	; yes, return state_rom
	bra	trr2
trr1:	movff	TBLPTRL,save_fsr0+0
	movff	TBLPTRH,save_fsr0+1
	movff	TBLPTRU,save_fsr0+2
	movff	TABLAT,_foo
	movlw	low _loader_code	; no, return eeprom byte
	addwf	BANKMASK(_state_romdone),w,c
	movwf	TBLPTRL,c
	clrf	TBLPTRH,c
	movlw	high _loader_code
	addwfc	TBLPTRH,f,c
	clrf	TBLPTRU,c		; there are 3 active tblptr bytes
	movlw	low (_loader_code shr 16)
	addwfc	TBLPTRU,f,c
	tblrd	*
	movf	TABLAT,w,c
	movwf	TRS_DATA_OUT,c
	movff	save_fsr0+0,TBLPTRL
	movff	save_fsr0+1,TBLPTRH
	movff	save_fsr0+2,TBLPTRU
	movff	_foo,TABLAT
trr2:	incf	BANKMASK(_state_romdone),f,c
	bra	int_ret_wr

trs_write_seccnt:
	movff	TRS_DATA_IN,_state_seccnt
	bra	int_ret_rd
	
trs_write_secnum:
	movff	TRS_DATA_IN,_state_secnum
	bra	int_ret_rd
	
trs_write_cyllo:
	movff	TRS_DATA_IN,_state_cyl
	bra	int_ret_rd
	
trs_write_cylhi:
	movf	TRS_DATA_IN,w,c		; cylinder is 10 bits
	andlw	0x03
	movwf	BANKMASK(_state_cyl+1),c
	bra	int_ret_rd
	
trs_write_sdh:
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_READY | TRS_HARD_SEEKDONE)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	clrf	BANKMASK(_state_secsize16+0),c
	clrf	BANKMASK(_state_secsize16+1),c
	rrncf	TRS_DATA_IN,w,c
	swapf	WREG,w,c
	andlw	0x3
	movwf	BANKMASK(_state_secsize),c		; trs_data_in[6:5]
	bnz	tws_01
	incf	BANKMASK(_state_secsize16+1),f,c	; 00 => 256 bytes   size16 = 0100h
	bra	tws_10
tws_01:	xorlw	0x1
	bnz	tws_02
	movlw	0x2
	movwf	BANKMASK(_state_secsize16+1),c	; 01 => 512 bytes   size16 = 0200h
	bra	tws_10	
tws_02:	xorlw	0x1 ^ 0x2
	bnz	tws_03
	movlw	0x4
	movwf	BANKMASK(_state_secsize16+1),c	; 10 => 1024 bytes  size16 = 0400h
	bra	tws_10
tws_03:	movlw	0x80
	movwf	BANKMASK(_state_secsize16+0),c	; 11 => 128 bytes   size16 = 0080h
tws_10:
	rlncf	TRS_DATA_IN,w,c
	swapf	WREG,w,c
	andlw	0x3
	movwf	BANKMASK(_state_drive),c		; trs_data_in[4:3]
	movf	TRS_DATA_IN,w,c
	andlw	0x7
	movwf	BANKMASK(_state_head),c		; trs_data_in[2:0]
	
	bra	int_ret_rd_S
	
trs_write_size2:
	clrf	BANKMASK(_state_size2+1),c
	movf	TRS_DATA_IN,w,c
	movwf	BANKMASK(_state_size2+0),c
	bnz	int_ret_rd
	incf	BANKMASK(_state_size2+1),f,c	; 0 --> 0x100
	bra	int_ret_rd

trs_write_cmd_restore:		; RESTORE
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_READY | TRS_HARD_SEEKDONE)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	clrf	BANKMASK(_state_cyl),c
	clrf	BANKMASK(_state_cyl+1),c
	bra	int_ret_rd_S

trs_write_cmd_read:		; READ
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_BUSY | TRS_HARD_CIP)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	movlw	ACTION_HARD_READ
	movwf	BANKMASK(_action_type),c
	bsf	BANKMASK(_action_flags),ACTION_TRS_BIT,c
	goto	int_ret_rd_S

trs_write_cmd_write:		; WRITE
	movf	TRS_DATA_IN,w,c	; we could probably skip this check
	andlw	TRS_HARD_MULTI
	bnz	twc_1
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_READY | TRS_HARD_SEEKDONE | TRS_HARD_DRQ)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	goto	int_ret_rd_S
twc_1:	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_READY | TRS_HARD_SEEKDONE | TRS_HARD_ERR)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	movlw	TRS_HARD_ABRTERR
	movwf	BANKMASK(_state_error),c
	goto	int_ret_rd_S
	
trs_write_cmd_verify:		; VERIFY
trs_write_cmd_seek:		; SEEK
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_BUSY | TRS_HARD_CIP)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	movlw	ACTION_HARD_SEEK
	movwf	BANKMASK(_action_type),c
	bsf	BANKMASK(_action_flags),ACTION_TRS_BIT,c
	goto	int_ret_rd_S

trs_write_cmd_nop:		; No Operation	
trs_write_cmd_format:		; FORMAT
trs_write_cmd_init:		; INIT
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_READY | TRS_HARD_SEEKDONE)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	goto	int_ret_rd_S

trs_write_cmd_test:			; TEST (WD-1002)
	clrf	BANKMASK(_state_error),c		; Clear error register
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	TRS_HARD_READY
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	goto	int_ret_rd_S

trs_write_command2:
	movff	TRS_DATA_IN,_state_command2
	clrf	BANKMASK(_state_bytesdone2),c
	movlw	ACTION_EXTRA
action_extraN:
	iorwf	BANKMASK(_state_command2),w,c
	movwf	BANKMASK(_action_type),c
	BANKSEL(SPI1CON2)
	movlw	(SPI1CON2_SPI1RXR_MASK | SPI1CON2_SPI1TXR_MASK)
	movwf	SPI1CON2,b
	movlw	1
	movwf	SPI1TCNTL,b
	movlw	(TRS_HARD_BUSY | TRS_HARD_CIP)
	movwf	SPI1TXB,b
	movwf	BANKMASK(_state_status),c
	bsf	BANKMASK(_action_flags),ACTION_TRS_BIT,c
	goto	int_ret_rd_S

trs_write_rom:
	movff	TRS_DATA_IN,_state_rom
	clrf	BANKMASK(_state_romdone),c
	goto	int_ret_rd

trs_write_uart_ctrl:  ; 	6 W C6
trs_write_uart:	     ; 	E W C7
	goto	int_ret_rd




	END
	
