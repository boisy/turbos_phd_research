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
dest           rmb       sourcesize*2
stack          rmb       100
size           equ       .

name           fcs       /rldec/
               fcb       edition


source         fcb       1,1,2,2,1,4,4,4
			   fcb       4,4,4,12,13,13,13,13
			   fcb       4,4,4,12,13,13,13,13
			   fcb       4,4,4,12,13,13,13,13,0

start:
forever	
	leax      source,pcr			point X to the source buffer
	leay      dest,u              point Y to the destination buffer
	ldd       #sourcesize         load D with the source size
 	bsr       rldec			    branch to the subroutine
*	ldx       #2				timed sleep 0 means sleep forever
*	os9		  F$Sleep			perform sleep waiting on a signal
	bra       forever			once we wake up, go back to sleep
	
    use       rldec.asm
	    
	emod
eom  equ       *
     end	    
