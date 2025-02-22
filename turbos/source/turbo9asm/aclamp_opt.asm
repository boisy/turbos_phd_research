*******************************************************************************
* TurbOS
*******************************************************************************
* See LICENSE.txt for licensing information.
********************************************************************
*
* $Id$
*
* Edt/Rev  YYYY/MM/DD  Modified by
* Comment
* ------------------------------------------------------------------
*   1      2024/05/31  BGP
* Created.

               nam       aclamp
               ttl       Audio clamp in Turbo9 assembly

               use       defs.d

tylg           set       Prgrm+Objct
atrv           set       ReEnt+rev
rev            set       $01
edition        set       1

               mod       eom,name,tylg,atrv,start,size

sourcesize     equ       32

               org       0
source         rmb       sourcesize
dest           rmb       sourcesize*2
stack          rmb       200
size           equ       .

name           fcs       /aclamp/
               fcb       edition

*
* RLE decoder.
*
* Example 1:
* 12 bytes of input: [1]A[1]B[1]A[3]C[1]A[1]B
*  8 bytes of output:  ABACCCABA
*
* Example 2:
* 12 bytes of input: [5]D[1]A[1]C[1]D[5]B[3]D
* 16 bytes of output:  DDDDDACDBBBBBDDD
*
	
start:
	leax	sighandler,pcr		point to the signal handler
	os9		F$Icpt			install it
	
forever	
	ldx       #0				timed sleep 0 means sleep forever
	os9		F$Sleep			perform sleep waiting on a signal
	bra       forever			once we wake up, go back to sleep
	
sighandler
	leax      source,u			point X to the source buffer
	leay      dest,u              point Y to the destination buffer
	ldd       #sourcesize         load D with the source size
*	bsr      	aclamp			branch to the subroutine
	rti						return from the signal handler
	
* Audio clamp in hand-optimized Turbo9 assembler
*
* Entry:
*    A = upper clamp value
*    B = lower clamp value
*    X = source buffer
*    Y = destination buffer
*    U = source buffer size
* Exit:
*    D = destination size
aclamp:
    pshs       d
l@  lda        ,x+
	cmpa       ,s
	ble        ok@
	lda        ,s
	bra        ok2@
ok@ cmpa       1,s
    bge        ok2@
	lda        1,s
ok2@ sta       ,y+
    leau       -1,u
	cmpu       #0
	bne        l@	
	puls       d,pc
    
	emod
eom  equ       *
     end	    
