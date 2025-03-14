************************************************************
* TurbOS
************************************************************
* See LICENSE.txt for licensing information.
************************************************************
*
* $Id$
*
* Edt/Rev  YYYY/MM/DD  Modified by
* Comment
* ----------------------------------------------------------
*   1      2024/05/31  Boisy Gene Pitre
* Created.

		nam     rldec
        ttl     Run-length decoder in Turbo9 assembly

        use     defs.d

tylg    set     Prgrm+Objct
atrv    set     ReEnt+rev
rev     set     $01
edition set     1

        mod     eom,name,tylg,atrv,start,size

        org    	0
dest    rmb     sourcel*2
stack   rmb     16
size    equ     .

name    fcs     /rldec/
        fcb     edition


source  fcb     1,1,2,2,1,4,4,4
		fcb     4,4,4,12,13,13,23,3
		fcb     2,28,1,19,13,163,3,44
		fcb     7,66,8,45,3,217,13,177,0
sourcel equ *-source			   

start:
forever	
		leax 	source,pcr	point X to the source buffer
		leay    dest,u      point Y to the destination buffer
		ldd     #sourcel	load D with the source size
 	   	bsr     rldec		branch to the subroutine
*		ldx     #2			timed sleep 0 means sleep forever
*		os9		F$Sleep		perform sleep waiting on a signal
		bra     forever		once we wake up, go back to sleep
	
    	use     rldec.asm
	    
		emod
eom  	equ       *
     	end	    
