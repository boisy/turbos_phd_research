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
dest           rmb       sourcesize*2
stack          rmb       100
size           equ       .

name           fcs       /aclamp/
               fcb       edition

source         fcb       1,6,9,18,19,21,2,8
			   fcb       9,15,23,12,11,15,16,7
               fcb       1,6,9,18,19,21,2,8
               fcb       9,15,23,12,11,15,16,7

start:
	
forever	
    ldd       #$03*256+12       load A/B with low/high clamp value
	leax      source,pcr		point X to the source buffer
    leay      dest,u            point Y to the destination buffer
    pshs      u                 save statics pointer
    ldu       #sourcesize       load U with the source size
 	bsr       aclamp			branch to the subroutine
    puls      u                 recover statics pointer
*	ldx       #2				timed sleep 0 means sleep forever
*	os9		  F$Sleep			perform sleep waiting on a signal
	bra       forever			once we wake up, go back to sleep
	
    use       aclamp.asm    
	
	emod
eom  equ       *
     end	    
