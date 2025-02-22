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

               nam       rldec
               ttl       Run-length decoder in Turbo9 assembly

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

name           fcs       /rldec/
               fcb       edition

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
*	bsr      	rldec			branch to the subroutine
	rti						return from the signal handler
	
    use       rldec.asm
	    
	emod
eom  equ       *
     end	    
