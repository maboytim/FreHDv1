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

	;; extern udata
	global	_crc7, _crc16, _foo

	psect	diskio_asm_data,class=COMRAM
xx:	ds	1


	psect	diskio_asm_code,class=CODE,space=0,reloc=2

;;;
;;; CRC-7  (x^7+x^3+x^0)
;;;
_mmc_crc7:
	GLOBAL	_mmc_crc7
	
	movwf	BANKMASK(xx),c
	movlw	8
	movwf	BANKMASK(_foo),c
mc0:	bcf	STATUS,0,c
	rlcf	BANKMASK(_crc7),c
	btfss	BANKMASK(xx),7,c
	bra	mc1
	movlw	0x09
	xorwf	BANKMASK(_crc7),f,c
mc1:	btfss	BANKMASK(_crc7),7,c
	bra	mc2
	movlw	0x09
	xorwf	BANKMASK(_crc7),c
mc2:	rlncf	BANKMASK(xx),c
	decfsz	BANKMASK(_foo),f,c
	bra	mc0

	return	0


;;;
;;; CRC-16  (x^16+x^12+x^5+x^0)
;;;
_mmc_crc16:
	GLOBAL	_mmc_crc16
	
	xorwf	BANKMASK(_crc16+1),w,c
	movwf	BANKMASK(_foo),c
	andlw	0xf0
	swapf	BANKMASK(_foo),f,c
	xorwf	BANKMASK(_foo),f,c
	movf	BANKMASK(_foo),w,c
	andlw	0xf0
	xorwf	BANKMASK(_crc16+0),w,c
	movwf	BANKMASK(_crc16+1),c
	rlncf	BANKMASK(_foo),w,c
	xorwf	BANKMASK(_crc16+1),f,c
	andlw	0xe0
	xorwf	BANKMASK(_crc16+1),f,c
	swapf	BANKMASK(_foo),f,c
	xorwf	BANKMASK(_foo),w,c
	movwf	BANKMASK(_crc16+0),c
	
	return	0

	end
