    list	p=18f47q83
#include <xc.inc>
#include "bootloader.h"

BF_FLASH_INVALID	equ	6	; true if flash has incorrect CRC16
BF_EEPROM_INVALID	equ	7	; true if eeprom has incorrect CRC16

	global	powerup,start,_flags,_crc
	psect	powerup,class=CODE,delta=1,reloc=2
powerup:
	nop	; Suggested Microchip errata workaround

	clrf	BANKMASK(_flags),c

;;;
;;; Compute CRC16 of 1000..1FFFD
;;;
do_crc16:
	movlw	low (BOOTLOADER_END+1)
	movwf	TBLPTRL,c
	movlw	high (BOOTLOADER_END+1)
	movwf	TBLPTRH,c
	movlw	low ((BOOTLOADER_END+1) shr 16)
	movwf	TBLPTRU,c

	clrf	BANKMASK(_crc+0),c
	clrf	BANKMASK(_crc+1),c
doc0:	tblrd*+
	movf	TABLAT,w,c
	rcall	crc16
	movf	TBLPTRU,w,c		; stop when TBLPTR = 01FFFE
	sublw	low(FLASH_CRC_ADDR shr 16)
	bnz	doc0
	movf	TBLPTRH,w,c		; stop when TBLPTR = 01FFFE
	sublw	high(FLASH_CRC_ADDR)
	bnz	doc0
	movf	TBLPTRL,w,c
	sublw	low(FLASH_CRC_ADDR)
	bnz	doc0

	tblrd*+
	movf	TABLAT,w,c
	cpfseq	BANKMASK(_crc+0),c		; lower crc match ?
	bsf	BANKMASK(_flags),BF_FLASH_INVALID,c	; no. set invalid flag
	tblrd*+
	movf	TABLAT,w,c
	cpfseq	BANKMASK(_crc+1),c		; upper crc match ?
	bsf	BANKMASK(_flags),BF_FLASH_INVALID,c	; no. set invalid flag
	btfsc	BANKMASK(_flags),BF_FLASH_INVALID,c	; invalid flash
	bra	enter_bootloader	; yes => bootloader mode

	movlw	low (0x380000+EEPROM_CRC_ADDR)
	movwf	TBLPTRL,c
	movlw	high (0x380000+EEPROM_CRC_ADDR)
	movwf	TBLPTRH,c
	movlw	low ((0x380000+EEPROM_CRC_ADDR) shr 16)
	movwf	TBLPTRU,c
	tblrd*+
	movf	TABLAT,w,c
	cpfseq	BANKMASK(_crc+0),c		; lower crc match ?
	bsf	BANKMASK(_flags),BF_EEPROM_INVALID,c	; no. set invalid flag
	tblrd*+
	movf	TABLAT,w,c
	cpfseq	BANKMASK(_crc+1),c		; upper crc match ?
	bsf	BANKMASK(_flags),BF_EEPROM_INVALID,c	; no. set invalid flag
	btfsc	BANKMASK(_flags),BF_EEPROM_INVALID,c	; invalid eeprom flag
	bra	enter_bootloader

enter_app_mode:
	goto	APP_STARTUP

enter_bootloader:
	goto	start

;;;
;;; CRC-16  (x^16+x^15+x^2+x^0)
;;;
crc16:
	xorwf	BANKMASK(_crc+0),w,c		; W = input xor old_crc+0
	xorwf	BANKMASK(_crc+1),w,c		; swap old_crc+1 with w
	xorwf	BANKMASK(_crc+1),f,c
	xorwf	BANKMASK(_crc+1),w,c		; new crc+1 = input xor old_crc+0
	movwf	BANKMASK(_crc+0),c		; new crc+0 = old_crc+1

	movf	BANKMASK(_crc+1),w,c		; save crc+1 in W
	swapf	BANKMASK(_crc+1),f,c		; trade nibbles
	xorwf	BANKMASK(_crc+1),f,c		; XOR high half byte with low
	rrcf	BANKMASK(_crc+1),f,c		; initialize carry
	btfsc	BANKMASK(_crc+1),0,c
	btg	STATUS,0,c	; compliment carry
	btfsc	BANKMASK(_crc+1),1,c
	btg	STATUS,0,c	; compliment carry
	btfsc	BANKMASK(_crc+1),2,c
	btg	STATUS,0,c	; compliment carry
	movwf	BANKMASK(_crc+1),c		; restore crc+1 from W

	movlw	0x1
	btfsc	STATUS,0,c	; if carry
	xorwf	BANKMASK(_crc+0),f,c		; flip bit 0 of crc+0
	movlw	0x40
	rrcf	BANKMASK(_crc+1),f,c		; shift parity into crc+1
	btfsc	STATUS,0,c	; if shift out is one
	xorwf	BANKMASK(_crc+0),f,c		; flip bit 6 of crc+0
	rlcf	BANKMASK(_crc+1),w,c		; unshift crc+1 into W
	xorwf	BANKMASK(_crc+1),f,c		; combine them
	rrcf	BANKMASK(_crc+1),f,c		; shift parity back into crc+1
	movlw	0x80
	btfsc	STATUS,0,c	; if shift out is one
	xorwf	BANKMASK(_crc+0),f,c		; flip bit 7 of crc+0
	retlw	0

	end
